
#include <QtCore>
#include "zipfile.h"

#include "zip.h"
#include "unzip.h"

#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef Q_WS_WIN
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#else
# define USEWIN32IOAPI
# include "iowin32.h"
# include <direct.h>
# include <io.h>
#endif

#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

class ZipFile::Private
{
public:
    Private(QString path) {
        containerPath = path;
        compressLevel = Z_DEFAULT_COMPRESSION;
        stop = false;
    }

    QString containerPath;

    zipFile wZipFile;
    int compressLevel;
    bool stop;
    
    QMap<QString, unz_file_info> filesInfo;
    
};

ZipFile::ZipFile(QString containerPath, QObject *parent ) 
:QObject(parent)
{
    d = new Private(containerPath);
    if (QFileInfo(containerPath).exists()) {
        unzFile zipFile = unzOpen(containerPath.toLocal8Bit());
        if (zipFile) {
            int res = unzGoToFirstFile(zipFile);
            while (res == UNZ_OK) {
                unz_file_info fileInfo;
                static char fileName[4096];
                unzGetCurrentFileInfo(zipFile, &fileInfo, fileName, sizeof(fileName),0,0,0,0 );
                d->filesInfo.insert( fileName, fileInfo );
                res = unzGoToNextFile(zipFile);
            }

            unzClose(zipFile);
        }
    }
}

ZipFile::~ZipFile() 
{
    delete d;
}

QString ZipFile::containerPath()
{
    return d->containerPath;
}

bool ZipFile::contains(QString nameFilter) const
{
    QStringList nameFilters( nameFilter );
    return !entryList( nameFilters ).empty();
}

QStringList ZipFile::entryList(QStringList nameFilters) const
{
    QStringList ls;

    foreach( QString nameFilter, nameFilters ) {
        QRegExp rx( nameFilter, Qt::CaseInsensitive, QRegExp::Wildcard );

        foreach( QString fileName, d->filesInfo.keys() ) {
            if ( rx.exactMatch(fileName) ) {
                ls.append( fileName);
            }
        }
    }

    return ls;
}

long ZipFile::entrySize(QString entryName) const
{
    if ( !d->filesInfo.contains(entryName) ) {
        return -1;
    }

    return d->filesInfo[entryName].uncompressed_size;
}

QIODevice * ZipFile::entry(QString entryName)
{
    unzFile zipFile = unzOpen(d->containerPath.toLocal8Bit());
    if (!zipFile) {
        return 0L;
    }

    if (  unzLocateFile( zipFile, entryName.toUtf8(), false) != UNZ_OK ) {
        unzClose(zipFile);
        return 0L; // no such file in zip
    }

    if ( unzOpenCurrentFile(zipFile) != UNZ_OK ) {
        unzClose(zipFile);
        return 0L; // can't open file
    }

    QBuffer * buffer = new QBuffer();
    buffer->open(QBuffer::ReadWrite);

    char data[1024];
    int readedBytes;

    while (1) {
        readedBytes = unzReadCurrentFile(zipFile, data, sizeof(data));

        if ( readedBytes > 0 ) {
            buffer->write(data,readedBytes);
        } else
            break;
    }

    unzCloseCurrentFile(zipFile);
    unzClose(zipFile);

    buffer->seek(0);
    return buffer;
}

/*!
 Adds a file to zip container.
 */
QIODevice * ZipFile::create(QString entryName, QDateTime creationTime)
{
    entryName = entryName.replace(QDir::separator(), "/");
    if ( entryName.startsWith("/") ) entryName = entryName.mid(1);

    bool exists = QFileInfo( d->containerPath ).exists();
    d->wZipFile = zipOpen( d->containerPath.toLocal8Bit(), exists ? 2 : 0);

    if ( !d->wZipFile ) 
        return 0L;

    zip_fileinfo zi;

    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;

    if ( !creationTime.isNull() ) {
        zi.tmz_date.tm_sec  = creationTime.time().second();
        zi.tmz_date.tm_min  = creationTime.time().minute();
        zi.tmz_date.tm_hour = creationTime.time().hour();
        zi.tmz_date.tm_mday = creationTime.date().day();
        zi.tmz_date.tm_mon  = creationTime.date().month()-1;
        zi.tmz_date.tm_year = creationTime.date().year();
    }

    QByteArray localEntryName = entryName.toLocal8Bit();

#ifdef Q_WS_WIN
    unsigned int codepage = ::GetOEMCP();
    
    QTextCodec * codec = QTextCodec::codecForName( QString("IBM %1").arg(codepage).toLatin1() );
    if (!codec) // second try
        codec = QTextCodec::codecForName( QString("Windows-%1").arg(codepage).toLatin1() );

    if (codec)
        localEntryName = codec->fromUnicode( entryName );
#endif

    int ok = zipOpenNewFileInZip3( d->wZipFile, localEntryName, &zi,
                                   NULL, 0, NULL, 0, NULL,
                                   (d->compressLevel != 0) ? Z_DEFLATED : 0,
                                   d->compressLevel, 0,
                                   -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                   NULL, 0);

    if ( ok != ZIP_OK ) {
        zipClose( d->wZipFile, NULL );
        return 0L;
    }

    QBuffer * buffer = new QBuffer(this);
    buffer->open(QBuffer::ReadWrite);

    connect( buffer, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)) );
    connect( buffer, SIGNAL(aboutToClose()), this, SLOT(closeInZip()) );

    return buffer;
}

bool ZipFile::addFile(QString filePath, QString entryName)
{
    QFileInfo fi(filePath);
    if (!fi.exists()) return false;
    if (fi.isDir()) {
        if (!entryName.endsWith("/")) entryName += "/";
        entryName += ".folder";
        return addFile(QByteArray(), entryName);
        
    }

    QIODevice * zipio = create( entryName, fi.lastModified() );

    if ( !zipio ) 
        return false;

    d->stop = false;
    QFile file( filePath );
    file.open(QIODevice::ReadOnly);
    qlonglong total = file.size();
    qlonglong done = 0;

    while ( !file.atEnd() && !d->stop ) {
        QByteArray bytes = file.read( 100*1024 );
        zipio->write( bytes );

        done += bytes.size();
        emit zipProgress( done, total);
        QCoreApplication::processEvents();
    }

    file.close();
    zipio->close();

    if ( d->stop ) 
        return false;

    QString message = tr("  Compressed into %1: %2").arg(QFileInfo(d->containerPath).fileName()).arg(entryName);
    qDebug() << message.toLocal8Bit().data();
    return true;
}

bool ZipFile::addFile(QByteArray fileData, QString entryName)
{
    QIODevice * zipio = create( entryName, QDateTime::currentDateTime() );

    //Q_ASSERT(zipio);

    //qDebug() << "add file" << entryName << fileData.count();

    if ( !zipio ) 
        return false;

    QBuffer buffer( &fileData );

    buffer.open(QIODevice::ReadOnly);
    qlonglong total = buffer.size();
    qlonglong done = 0;

    while ( !buffer.atEnd() && !d->stop ) {
        QByteArray bytes = buffer.read( 100*1024 );
        zipio->write( bytes );

        done += bytes.size();
        emit zipProgress( done, total);
        QCoreApplication::processEvents();
    }

    buffer.close();
    zipio->close();
    return true;
}

int ZipFile::compressLevel()
{
    return d->compressLevel;
}

void ZipFile::setCompressLevel(int level)
{
    d->compressLevel = level;
}

void ZipFile::bytesWritten(qint64 bytes)
{
    if ( !d->wZipFile ) return;
    QBuffer * buffer = (QBuffer *)sender();

    printf(".");
    fflush(stdout);

    buffer->seek(0);
    QByteArray ba = buffer->read(bytes);
    int ok = zipWriteInFileInZip ( d->wZipFile , ba.data(), bytes);
    buffer->seek(0);

    if ( ok<0 )
    {
        qDebug() << tr("Error in writing zipfile %1").arg( d->containerPath );
        return;
    }
}

void ZipFile::closeInZip()
{
    if ( d->wZipFile ) {
        zipCloseFileInZip( d->wZipFile );
        zipClose( d->wZipFile, NULL);
    }
}

/*!
 Stops addFile method
 */
void ZipFile::stop()
{
    d->stop = true;
}

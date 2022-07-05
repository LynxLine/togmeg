//
// Copyright Petrostreamz 2009
//

#include <time.h>
#include <QtCore>
#include "MiscUtils.h"

#ifndef Q_WS_WIN
    #include <unistd.h>
#endif

QString MiscUtils::userName()
{
#ifdef Q_WS_WIN
        return QString::fromLocal8Bit(getenv("USERNAME"));
#else
        return QString::fromLocal8Bit(getenv("USER"));
#endif
}

QString MiscUtils::hostName()
{
#ifdef Q_WS_WIN
  	QString host = QString::fromLocal8Bit(getenv("USERDNSDOMAIN"));
    if (host.isEmpty())
        host = QString::fromLocal8Bit(getenv("USERDOMAIN"));
    if (host.isEmpty())
        host = QString::fromLocal8Bit(getenv("COMPUTERNAME"));
#else
    QString host;
    char c_host[256];
    if (gethostname(c_host, 256) == 0)
        host = QString::fromLocal8Bit(c_host);
#endif
    if (host.isEmpty())
        host = "localhost";

    return host;
}

QString MiscUtils::userAtHostName()
{
    return userName()+"@"+hostName();
}

void MiscUtils::showInFileManager(QString filePath)
{
#ifdef Q_WS_MAC
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "select POSIX file \""+filePath+"\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
#endif
    
#ifdef Q_WS_WIN
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(filePath);
    QProcess::startDetached("explorer", args);
#endif
    
}

void MiscUtils::moveFileToTrash(QString filePath)
{
#ifdef Q_WS_MAC
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "delete POSIX file \""+filePath+"\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
#endif
}

void MiscUtils::copyFolder(QString sourcePath, QString destPath)
{
    QDir::root().mkpath(destPath);
    
#ifdef Q_WS_MAC
    QStringList args;
    args << "-e";
    args << "set ss to \""+sourcePath+"\"";
    args << "-e";
    args << "set ps to POSIX file ss";
    args << "-e";
    args << "set sc to \""+destPath+"\"";
    args << "-e";
    args << "set pc to POSIX file sc";
    args << "-e";
    args << "tell application \"Finder\" to duplicate items "\
            "of folder ps to folder pc with replacing";
    QProcess::startDetached("osascript", args);
#endif
    
#ifdef Q_WS_WIN
    QString path = QCoreApplication::applicationDirPath();
    path += "/Utilities/CopyFolder.vbs";
    QStringList args;
    args << QDir::toNativeSeparators(path);
    args << QDir::toNativeSeparators(sourcePath);
    args << QDir::toNativeSeparators(destPath);
    QProcess::startDetached("wscript", args);
#endif
    
}

QString MiscUtils::buildId()
{
    QString embeddedDate(__DATE__);
    QString m = embeddedDate.left(3);
    QStringList months;
    months << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun"
           << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    
    int month = months.indexOf(m) + 1;
    int day   = embeddedDate.mid(4, 2).trimmed().toInt();
    int year  = embeddedDate.mid(7, 4).trimmed().toInt();
    
    return QString::number(year*10000+month*100+day);
}

QStringList MiscUtils::splitWithRespectQuotes(QString text)
{
    QStringList res;

    QRegExp rxs("\\s");
    QRegExp rxns("\\S");
    int pos = -1;

    while((pos = text.indexOf(rxns, pos+1)) >=0) {
        QChar ch = text[pos];
        if (ch == '"' || ch == '\'') {
            int pos2 = text.indexOf(ch, pos+1);
            if (pos2 >= 0) {
                res << text.mid(pos+1, pos2-pos-2);
                pos = pos2;
            }
            else {
                res << text.mid(pos+1);
                break;
            }
        }
        else {
            int pos2 = text.indexOf(rxs, pos+1);
            if (pos2 >= 0) {
                res << text.mid(pos, pos2-pos);
                pos = pos2;
            }
            else {
                res << text.mid(pos+1);
                break;
            }
        }
    }

    if (!res.count())
        res << text.trimmed();

    return res;
}

/*!
 Returns unique string ID of required length.
 Contains [0-9A-F] symbols.
 */
QString MiscUtils::uniqueId(int length)
{
    static bool initialized = false;
    if ( !initialized ) {
        initialized = true;
        srand(time(NULL));
    }
    
    QString id;
    QStringList slist;
    slist << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7"
          << "8" << "9" << "A" << "B" << "C" << "D" << "E" << "F";
    
    for (int i=0;i<length;i++) {
        id += slist[ rand() % 16 ];
    }
    
    return id;
}

#ifdef Q_WS_WIN
	#include <qt_windows.h>
	#include <tlhelp32.h>
#endif

bool MiscUtils::isProcessRunning(qint64 pid)
{
#ifdef Q_WS_WIN
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE ) {
		qDebug() << "Error: CreateToolhelp32Snapshot";
		return true;
	}

	pe32.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( hProcessSnap, &pe32 ) ) {
		qDebug() << "Error: Process32First";
		CloseHandle( hProcessSnap ); // clean the snapshot object
		return true;
	}

    bool found = false;
	do {
		if (pid == pe32.th32ProcessID) found = true;
	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
    return found;
#else
    //Can we get proc info by pid on Mac (C routines?)
    Q_UNUSED(pid);
    return true;
#endif
}

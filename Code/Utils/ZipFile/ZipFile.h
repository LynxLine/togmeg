
#ifndef ZipFile_H
#define ZipFile_H

#include <QObject>
#include <QDateTime>
#include <QIODevice>

class ZipFile : public QObject
{
Q_OBJECT
public:
    ZipFile(QString containerPath, QObject * parent=0);
    virtual ~ZipFile();

    QString containerPath();
    
    bool contains(QString nameFilter) const;
    long entrySize(QString entryName) const;
    QStringList entryList(QStringList nameFilters) const;
    
    QIODevice * create(QString entryName, QDateTime creationTime = QDateTime());
    QIODevice * entry(QString entryName);

    int compressLevel();
    void setCompressLevel(int);

    bool addFile(QString filePath, QString entryName);
    bool addFile(QByteArray fileData, QString entryName);
    void stop();

signals:
    void zipProgress(qlonglong done, qlonglong total);

protected slots:
    void bytesWritten(qint64 bytes);
    void closeInZip();

private:
    class Private;
    Private * d;
};

#endif // ZipFile_H


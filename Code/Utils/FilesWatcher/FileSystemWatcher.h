//
// Copyright (C) 2009 Alexander Strelets <alexander.strelets@gmail.com>
//

#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include<QThread>
#include<QString>
#include<QStringList>

class FileSystemWatcher : public QThread {
Q_OBJECT

public:
    FileSystemWatcher(QObject * parent = 0L,
                      quint16 theMaxDuration = 60,     /* Time limit for active phase (msec), 0 means unlimited  */
                      quint16 theMinPause    = 1000,   /* Minimum for passive phase (msec), 1000 means 1sec      */
                      quint16 theMultiplier  = 1000);  /* Ratio between passive and active phase                 */

    ~FileSystemWatcher();

    void addFile(const QString & path);
    void addFiles(const QStringList & paths);

    void removeFile(const QString & path);
    void removeFiles(const QStringList & paths);

    void clear();
    void stop();
signals:
    void fileChanged(QString path);
    void fileCreated(QString path);
    void fileDeleted(QString path);
    //void fileDoesNotExist(QString path);

protected:
    void run();

private:
    class FileInfo;
    class Private;
    Private * d;    
};

#endif // FILESYSTEMWATCHER_H

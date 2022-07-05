//
// Copyright (C) 2009 Alexander Strelets <alexander.strelets@gmail.com>
//

#ifndef FILESWATCHER_H
#define FILESWATCHER_H

#include<QString>
#include<QStringList>

class FilesWatcher : public QObject {
Q_OBJECT

public:
    FilesWatcher(QObject * parent = 0L);
    ~FilesWatcher();

    void addFile(const QString & path);
    void addFiles(const QStringList & paths);

    void removeFile(const QString & path);
    void removeFiles(const QStringList & paths);

    void clear();
signals:
    void fileChanged(QString path);
    void fileCreated(QString path);
    void fileDeleted(QString path);
    //void fileDoesNotExist(QString  path);

private slots:
    void filterFileChanged(QString path);
    void filterFileCreated(QString path);
    void filterFileDeleted(QString path);
    //void filterFileDoesNotExist(QString path);

private:
    class Private;
    Private * d;    
};

#endif // FILESWATCHER_H

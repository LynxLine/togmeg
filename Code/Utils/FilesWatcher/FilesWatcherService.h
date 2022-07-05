#ifndef SHAREDFILESYSTEMWATCHER_H
#define SHAREDFILESYSTEMWATCHER_H

#include "FileSystemWatcher.h"


#include<QThread>
#include<QString>
#include<QStringList>

class FilesWatcherService : public FileSystemWatcher {
Q_OBJECT

protected:
    FilesWatcherService(QObject * parent);
    ~FilesWatcherService();

public:
    static FilesWatcherService* instance();

    void addFile(const QObject *handler, const QString & path);
    void addFiles(const QObject *handler, const QStringList & paths);

    void removeFile(const QObject *handler, const QString & path);
    void removeFiles(const QObject *handler, const QStringList & paths);

public slots:
    void clear(QObject *handler);

private:

    class Private;
    Private * d;    
};

#endif // SHAREDFILESYSTEMWATCHER_H

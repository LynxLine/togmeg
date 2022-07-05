//
// Copyright Petrostreamz 2009
//

#include <QtCore>
#include "FilesWatcher.h"
#include "FilesWatcherService.h"

class FilesWatcher::Private {
 public:
    FilesWatcherService *service;
    QMap<QString, bool> watched;
};

FilesWatcher::FilesWatcher(QObject * parent)
    : QObject(parent)
{
    d = new Private;
    d->service = FilesWatcherService::instance();
    
    connect(d->service, SIGNAL(fileChanged(const QString &)),
            this, SLOT(filterFileChanged(const QString &)));

    connect(d->service, SIGNAL(fileCreated(const QString &)),
            this, SLOT(filterFileCreated(const QString &)));

    connect(d->service, SIGNAL(fileDeleted(const QString &)),
            this, SLOT(filterFileDeleted(const QString &)));

/*
    connect(d->service, SIGNAL(fileDoesNotExist(const QString &)),
            this, SLOT(filterFileDoesNotExist(const QString &)));
*/
}

FilesWatcher::~FilesWatcher()
{
    delete d;
}


void FilesWatcher::addFile(const QString &path)
{
    if(!d->watched.value(path, false)) {
       d->service->addFile(this, path);
       d->watched[path] = true;
    }
}

void FilesWatcher::addFiles(const QStringList &paths)
{
    QString path;
    foreach(path, paths) addFile(path);
}

void FilesWatcher::removeFile(const QString &path)
{
    if(d->watched.value(path, false)) {
        d->service->removeFile(this, path);
        d->watched[path] = true;
    }
}

void FilesWatcher::removeFiles(const QStringList & paths)
{
    QString path;
    foreach(path, paths) removeFile(path);
}

void FilesWatcher::clear()
{
    d->service->clear(this);
}

void FilesWatcher::filterFileChanged(QString path)
{
    if(d->watched.value(path, false)) {
        emit fileChanged(path);
    }
}

void FilesWatcher::filterFileCreated(QString path)
{
    if(d->watched.value(path, false)) {
        emit fileCreated(path);
    }
}

void FilesWatcher::filterFileDeleted(QString path)
{
    if(d->watched.value(path, false)) {
        emit fileDeleted(path);
    }
}

/*
void FilesWatcher::filterFileDoesNotExist(QString path)
{
    if(d->watched.value(path, false)) {
        emit fileDoesNotExist(path);
    }
}
*/

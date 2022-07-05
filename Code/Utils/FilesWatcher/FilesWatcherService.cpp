#include <QCoreApplication>

#include "FilesWatcherService.h"

#include <QMultiMap>

class FilesWatcherService::Private {
  public:
    QMultiMap<const QObject *, QString > watched_by;
    QMultiMap<QString, const QObject * > watches_for;
    static FilesWatcherService *instance;
};

FilesWatcherService *FilesWatcherService::Private::instance = 0;

FilesWatcherService::FilesWatcherService(QObject * parent)
 : FileSystemWatcher(parent, 100, 1000, 25) 
{
    d = new Private();
    start(QThread::LowPriority);
}

FilesWatcherService::~FilesWatcherService() 
{
    delete d;
}

FilesWatcherService * FilesWatcherService::instance() 
{
    if(!Private::instance) {
        Private::instance = new FilesWatcherService(qApp);
    }
    return Private::instance;
}

void FilesWatcherService::addFile(const QObject *handler, const QString &filePath) {
    if (d->watches_for.contains(filePath, handler)) return;

    if (!d->watches_for.contains(filePath)) {
        FileSystemWatcher::addFile(filePath);
    }

    if (!d->watched_by.contains(handler)) {
        connect(handler, SIGNAL(destroyed(QObject*)),
                this, SLOT(clear(QObject *)));
    }
            
    d->watches_for.insert(filePath, handler);
    d->watched_by.insert(handler, filePath);
}

void  FilesWatcherService::addFiles(const QObject *handler, const QStringList & paths) {
    QString path;
    foreach(path, paths) addFile(handler, path);
}

void FilesWatcherService::removeFile(const QObject *handler, const QString &filePath) {
    if (!d->watches_for.contains(filePath, handler)) return;

    d->watches_for.remove(filePath, handler);
    d->watched_by.remove(handler, filePath);

    if (!d->watched_by.contains(handler)) {
        disconnect(handler, SIGNAL(destroyed(QObject*)),
                   this, SLOT(clear(QObject *)));
    }

    if (!d->watches_for.contains(filePath)) {
        FileSystemWatcher::removeFile(filePath);
    }
}

void  FilesWatcherService::removeFiles(const QObject *handler, const QStringList & paths) {
    QString path;
    foreach(path, paths) removeFile(handler, path);
}

void FilesWatcherService::clear(QObject *handler) {
    foreach (QString filePath, d->watched_by.values(handler)) {
        removeFile(handler, filePath);
    }
}


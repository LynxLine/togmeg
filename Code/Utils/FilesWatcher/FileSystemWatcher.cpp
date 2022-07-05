//
// Copyright Petrostreamz 2009
//

#include <QtCore>
#include "FileSystemWatcher.h"

class FileSystemWatcher::FileInfo : public QFileInfo {
 public:
    FileInfo(const QString & file) : QFileInfo(file) { 
        this->setCaching(false);
        if(this->exists()) {
            prevLastModified = this->lastModified();
        }
    }
    ~FileInfo() {}
    bool isChanged() {
        QDateTime lastModified = this->lastModified();
        if (prevLastModified < lastModified) {
            prevLastModified = lastModified;
            return true;
        }
        return false;
    };
    bool isCreated() {
        if (!prevLastModified.isValid() && this->exists()) {
            prevLastModified = this->lastModified();
            return true;
        }
        return false;
    };
    bool isDeleted() {
        if (prevLastModified.isValid() && !this->exists()) {
            prevLastModified = QDateTime();
            return true;
        }
        return false;
    };
private:
    QDateTime prevLastModified;
};

class FileSystemWatcher::Private {
public:
    FileSystemWatcher * watcher;
    quint16 multiplier;
    quint16 minPause;
    quint16 maxDuration;
    
    QMap<QString, FileInfo *> fileList;
    QMap<QString, FileInfo *>::iterator current;
    
    QMutex mutexAdd;
    QStringList filesToAdd;
    
    QMutex mutexRemove;
    QStringList filesToRemove;
    
    volatile bool clear;
    volatile bool stop;
    
    void th_addFiles();
    void th_removeFiles();
    bool th_clear();
    int  th_checkFiles();    
};

FileSystemWatcher::FileSystemWatcher(QObject * parent,
                                     quint16 theMaxDuration, quint16 theMinPause, quint16 theMultiplier)
    : QThread(parent)
{
    d = new Private;
    d->watcher = this;
    d->multiplier  = theMultiplier;
    d->minPause    = theMinPause;
    d->maxDuration = theMaxDuration;
    d->stop        = false;
    d->clear       = false;
    d->current     = d->fileList.end();
}

FileSystemWatcher::~FileSystemWatcher()
{
    clear();
    // start it to proceed clearing
    if(!isRunning()) start();
    stop();
    wait();
    delete d;
}


void FileSystemWatcher::addFile(const QString &path)
{
    QMutexLocker locker(&d->mutexAdd);
    this->d->filesToAdd.push_back(path);
}

void FileSystemWatcher::addFiles(const QStringList &paths)
{
    QMutexLocker locker(&d->mutexAdd);
    QString path;
    foreach(path, paths) d->filesToAdd.push_back(path);
}

void FileSystemWatcher::removeFile(const QString &path)
{
    QMutexLocker locker(&d->mutexRemove);
    d->filesToRemove.push_back(path);
}

void FileSystemWatcher::removeFiles(const QStringList & paths)
{
    QMutexLocker locker(&d->mutexRemove);
    QString path;
    foreach(path, paths) d->filesToRemove.push_back(path);
}

void FileSystemWatcher::clear()
{
    QMutexLocker locker1(&d->mutexRemove);
    QMutexLocker locker2(&d->mutexAdd);
    d->clear = true;
    d->filesToAdd.clear();
}

void FileSystemWatcher::stop()
{
    QMutexLocker locker1(&d->mutexRemove);
    QMutexLocker locker2(&d->mutexAdd);
    if(isRunning()) d->stop = true;
}

void FileSystemWatcher::run() 
{
    const int pause_quantum = 1000;
    forever {

        if (d->th_clear()) return;
        // active phase
        d->th_removeFiles();
        int duration = d->th_checkFiles();
        //qDebug() << "Duration:" << duration ;
        d->th_addFiles();
        int pause = duration*d->multiplier;
        if ( pause < d->minPause ) {
            pause = d->minPause;
        }
        //qDebug() << "Pause:" << pause ;

        // passive phase
        int n = int(pause/pause_quantum);
        for (int s=0; s < n; s++) {
            if (d->th_clear()) return;
            msleep(pause_quantum);
        }
        int r = pause%pause_quantum;
        if (r > 0) {
            if (d->th_clear()) return;
            msleep(r);
        }
    }
}

/* Internal fuctions */

void FileSystemWatcher::Private::th_addFiles() {
    QMutexLocker locker(&mutexAdd);
    while(filesToAdd.size() > 0) {
        QString path = filesToAdd.takeFirst();
        if (fileList.contains(path)) continue;
        fileList.insert(path, new FileInfo(path)); 
        //qDebug() << "Added file:" << path;
   }
}

void FileSystemWatcher::Private::th_removeFiles() {
    QMutexLocker locker(&mutexRemove);
    while(filesToRemove.size() > 0) {
        QString path = filesToRemove.takeFirst();
        QMap<QString, FileInfo *>::iterator i = fileList.find(path);
        if (i != fileList.end()) {
            FileInfo *fileInfo = i.value();
            if (i == current) {
                current = fileList.erase(current);
            } else {
                fileList.erase(i);
            }
            delete fileInfo;
            //qDebug() << "Removed file:" << path;
        }
    }
}

bool FileSystemWatcher::Private::th_clear() {
    QMutexLocker locker1(&mutexAdd);
    QMutexLocker locker2(&mutexRemove);
    if (clear) {
        clear = false;
        filesToRemove.clear();
        QMap<QString, FileInfo *>::iterator i = fileList.begin();
        while (i != fileList.end()) {
            FileInfo *fileInfo = i.value();
            i = fileList.erase(i);
            delete fileInfo;
        }
        current = fileList.end();
        //qDebug() << "Cleared";
    }
    if (stop) {
        //qDebug() << "Stoped";
        stop=false;
        return true;
    }
    return false;
}


int FileSystemWatcher::Private::th_checkFiles() 
{
    if (current == fileList.end()) {
        //qDebug() << "Reset";
        current = fileList.begin();
    }
    //qDebug() << "Now:" << QDateTime::currentDateTime();
    int count = 0;
    QTime timer;
    timer.start();
    while (current != fileList.end()) {
        FileInfo *fileInfo = current.value();
        if (fileInfo->isCreated()) {
            //qDebug() << "fileCreated" << current.key();
            emit watcher->fileCreated(current.key());
        }
        else if (fileInfo->isDeleted()) {
            //qDebug() << "filedDeleted" << current.key();
            emit watcher->fileDeleted(current.key());        
        }
/*
        if (!fileInfo->exists()) {
            emit watcher->fileDoesNotExist(current.key());
        } 
*/
        else if (fileInfo->isChanged()) {
            //qDebug() << "fileChanged" << current.key();
            emit watcher->fileChanged(current.key());
        }
        current++;
        count ++;
        if (maxDuration > 0) {
            int duration = timer.elapsed();
            if ( duration >= maxDuration) {
                //qDebug() << "Count:" << count;
                return duration;
            }
        }
    }
    //qDebug() << "Count" << count;
    //qDebug() << "End";
    return timer.elapsed();
}

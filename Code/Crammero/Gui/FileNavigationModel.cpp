//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "FileNavigationModel.h"

#include "IconSet.h"

class FileNavigationModel::Private {
public:
    QString rootPath;
    QString path;
    QFileInfoList entries;
    QFileSystemWatcher watcher;
    QFileIconProvider iconProvider;
};

/*!
 * Creates the object.
 */
FileNavigationModel::FileNavigationModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
    connect(&d->watcher, SIGNAL(directoryChanged(const QString &)),
            this, SLOT(reload()));
}

/*!
 * Deletes it.
 */
FileNavigationModel::~FileNavigationModel()
{
    delete d;
}

int FileNavigationModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->entries.count();
}

int FileNavigationModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return ColCount;
}

QVariant FileNavigationModel::data(const QModelIndex & index, int role) const
{
    if ( !index.isValid() ) return QVariant();
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return QVariant();
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( index.column() == ColName ) 
            return d->entries[ index.row() ].fileName();
        else if ( index.column() == ColLinks ) {
            int linkCount = 0;
            return linkCount;
        }
    }
    
    if (role == Qt::DecorationRole) {
        if ( index.column() == ColName )
            if (d->entries[ index.row() ].isDir())
                return d->iconProvider.icon( d->entries[ index.row() ].filePath() );
    }
    
    if (role == Qt::TextAlignmentRole && index.column() == ColName) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::TextAlignmentRole && index.column() == ColLinks) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    
    return QVariant();
}

QVariant FileNavigationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);
    
    if (role == Qt::DisplayRole) {
        if (section == ColName)   return tr("Name");
    }
    
    return QVariant();
}

QFileInfo * FileNavigationModel::fileInfo(const QModelIndex & i) const
{
    if (!i.isValid()) return 0L;
    return &d->entries[i.row()];
}

QString FileNavigationModel::rootPath() const
{
    return d->rootPath;
}

QString FileNavigationModel::path() const
{
    return d->path;
}

void FileNavigationModel::reload()
{
    loadPathContent(path());
}

void FileNavigationModel::setRootPath(const QString & path)
{
    if (path.isEmpty()) {
        d->entries.clear();
        d->rootPath.clear();
        d->path.clear();
        reset();
        return;
    }
        
    d->rootPath = QDir::fromNativeSeparators(path);
    if (!d->rootPath.endsWith("/"))
        d->rootPath += "/";
    
    loadPathContent(QString::null);
}

void FileNavigationModel::loadPathContent(const QString & p)
{
    QString pp = p;
    pp = QDir::fromNativeSeparators(pp);
    if (pp.startsWith("/"))
        pp = pp.mid(1);

    QString fp = QFileInfo(rootPath()+pp).canonicalFilePath();
    if (!fp.endsWith("/"))
        fp += "/";

    if (!fp.startsWith(rootPath()))
        return;
    
    d->path = fp.mid(rootPath().length());

    QDir dir(rootPath()+path());
    d->entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
    if (!path().isEmpty())
        d->entries.prepend(QFileInfo(rootPath()+".."));
    reset();

    if (d->watcher.files().count())
        d->watcher.removePaths( d->watcher.files() );
    if (d->watcher.directories().count())
        d->watcher.removePaths( d->watcher.directories() );
    
    d->watcher.addPath(rootPath()+path());
}

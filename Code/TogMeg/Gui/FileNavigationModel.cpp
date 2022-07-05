
#include <QtGui>
#include "FileNavigationModel.h"

#include "IconSet.h"

class FileNavigationModel::Private {
public:
    QString rootPath;
    QString path;
    QFileInfoList dirs;
    QFileInfoList files;
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

Qt::ItemFlags FileNavigationModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= rowCount() ) return Qt::ItemIsDropEnabled;
    if ( index.column() == ColName && index.data().toString() == "..")
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

int FileNavigationModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->dirs.count()+d->files.count();
}

int FileNavigationModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return ColCount;
}

QVariant FileNavigationModel::data(const QModelIndex & i, int role) const
{
    if ( !i.isValid() ) return QVariant();
    if ( i.row() < 0 || i.row() >= rowCount() ) return QVariant();
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( i.column() == ColName ) {
            QString n = i.row() < d->dirs.count() ?
                        d->dirs[i.row()].fileName() :
                        d->files[i.row()-d->dirs.count()].fileName();

            if (n.endsWith(".tab", Qt::CaseInsensitive))
                n = n.left(n.length()-4);

            return n;
        }
    }
    
    if (role == Qt::DecorationRole) {
        if ( i.column() == ColName )
            if (i.row() < d->dirs.count())
                return d->iconProvider.icon( d->dirs[ i.row() ].filePath() );
            else {
                QPixmap pm(16,16);
                pm.fill(QColor(0,0,0, 0));
                return QIcon(pm);
            }
    }
    
    if (role == Qt::TextAlignmentRole && i.column() == ColName) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    
    if (role == AbsFilePathRole) {
        return i.row() < d->dirs.count() ?
               d->dirs[i.row()].absoluteFilePath() :
               d->files[i.row()-d->dirs.count()].absoluteFilePath();

    }

    return QVariant();
}

bool FileNavigationModel::setData(const QModelIndex & i, const QVariant & value, int role)
{
    if ( !i.isValid() ) return false;
    if ( i.row() < 0 || i.row() >= rowCount() ) return false;
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (i.column() == ColName) {
            QString n = value.toString();

            if (i.row() >= d->dirs.count() &&
                !n.endsWith(".tab"))
                n += ".tab";
            
            QFile f(i.data(AbsFilePathRole).toString());
            bool ok = f.rename(n);
            if (!ok) return false;

            if (i.row() < d->dirs.count())
                d->dirs[i.row()].setFile(f);
            else d->files[i.row()-d->dirs.count()].setFile(f);

            emit dataChanged(i,i);
            return true;
        }
    }
    
    return false;
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
    if (i.row() < d->dirs.count())
        return &d->dirs[i.row()];
    else return &d->files[i.row()-d->dirs.count()];
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
        d->files.clear();
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

void FileNavigationModel::syncSortedItems(QFileInfoList & oldItems,
                                          QFileInfoList & newItems, int shift)
{
    int idx = 0;
    QFileInfoList::iterator n_it = newItems.begin();
    QFileInfoList::iterator o_it = oldItems.begin();
    while(n_it != newItems.end() ||
          o_it != oldItems.end()) {

        if (n_it != newItems.end() &&
            o_it != oldItems.end() &&
            *n_it == *o_it) {
            n_it++;
            o_it++;
            idx++;
        }
        else {
            if (o_it == oldItems.end()) {
                beginInsertRows(QModelIndex(), idx+shift, idx+shift);
                o_it = oldItems.insert(o_it, *n_it);
                endInsertRows();

                o_it++;
                n_it++;
                idx++;
            }
            else if (n_it == newItems.end()) {
                beginRemoveRows(QModelIndex(), idx+shift,idx+shift);
                o_it = oldItems.erase(o_it);
                endRemoveRows();
            }
            else if ((*n_it).fileName() < (*o_it).fileName()) {
                beginInsertRows(QModelIndex(), idx+shift,idx+shift);
                o_it = oldItems.insert(o_it, *n_it);
                endInsertRows();

                o_it++;
                n_it++;
                idx++;
            }
            else if ((*n_it).fileName() > (*o_it).fileName()) {
                beginRemoveRows(QModelIndex(), idx+shift,idx+shift);
                o_it = oldItems.erase(o_it);
                endRemoveRows();
            }
            else {
                *(o_it) = *(n_it);
                emit dataChanged(index(idx+shift,0), index(idx+shift,columnCount()-1));
                n_it++;
                o_it++;
                idx++;
            }
        }
    }
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
    
    bool reload = true;
    QString new_path = fp.mid(rootPath().length());
    if (d->path != new_path) {
        emit aboutToLoad();
        reload = false;
    }
    d->path = new_path;

    QDir dir(rootPath()+path());
    QStringList nameFilters;
    nameFilters << "*.tab" << "*.xml";

    QFileInfoList dirs, files;
    dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
    files = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);

    if (!path().isEmpty())
        dirs.prepend(QFileInfo(rootPath()+".."));

    syncSortedItems(d->dirs, dirs, 0);
    syncSortedItems(d->files, files, d->dirs.count());

    if (!reload)
        emit loaded();
	else emit reloaded();

    if (d->watcher.files().count())
        d->watcher.removePaths( d->watcher.files() );
    if (d->watcher.directories().count())
        d->watcher.removePaths( d->watcher.directories() );
    
    d->watcher.addPath(rootPath()+path());   
}

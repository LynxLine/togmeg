
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

Qt::ItemFlags FileNavigationModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return Qt::ItemIsDropEnabled;
    if ( index.column() == ColName && index.data().toString() == "..")
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
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

QVariant FileNavigationModel::data(const QModelIndex & i, int role) const
{
    if ( !i.isValid() ) return QVariant();
    if ( i.row() < 0 || i.row() >= d->entries.count() ) return QVariant();
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( i.column() == ColName ) {
            QString fileName = d->entries[ i.row() ].fileName();
            if (fileName.endsWith(".tab", Qt::CaseInsensitive))
                fileName = fileName.left(fileName.length()-4);
            return fileName;
        }
    }
    
    if (role == Qt::DecorationRole) {
        if ( i.column() == ColName )
            if (d->entries[ i.row() ].isDir())
                return d->iconProvider.icon( d->entries[ i.row() ].filePath() );
            else {
                QPixmap pm(16,16);
                pm.fill(QColor(0,0,0, 0));
                return QIcon(pm);
            }
    }
    
    if (role == Qt::TextAlignmentRole && i.column() == ColName) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    
    return QVariant();
}

bool FileNavigationModel::setData(const QModelIndex & i, const QVariant & value, int role)
{
    if ( !i.isValid() ) return false;
    if ( i.row() < 0 || i.row() >= d->entries.count() ) return false;
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (i.column() == ColName) {
            QString newName = value.toString();
            if (d->entries[i.row()].isFile() && !newName.endsWith(".tab"))
                newName += ".tab";
            
            QFile f(d->entries[i.row()].absoluteFilePath());
            bool ok = f.rename(newName);
            if (!ok) return false;

            d->entries[i.row()].setFile(f);
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
    QStringList nameFilters;
    nameFilters << "*.tab" << "*.xml";

    QFileInfoList entries;
    entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
    entries += dir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);

    if (!path().isEmpty())
        entries.prepend(QFileInfo(rootPath()+".."));

    /*
    d->entries = entries;
    reset();
     */

    bool in_add = false;
    bool in_rem = false;
    QFileInfoList::iterator n_it = entries.begin();
    QFileInfoList::iterator o_it = d->entries.begin();
    while(n_it != entries.end() || o_it != d->entries.end()) {

        //if (n_it != entries.end()) qDebug() << (*n_it).fileName(); else qDebug() << "n_it - end";
        //if (o_it != d->entries.end()) qDebug() << (*o_it).fileName(); else qDebug() << "o_it - end";

        if (n_it != entries.end() &&
            o_it != d->entries.end() &&
            *n_it == *o_it) {
            n_it++;
            o_it++;
            //if (in_add) {endInsertRows();in_add = false;}
            //if (in_rem) {endRemoveRows();in_rem = false;}
        }
        else {
            if (o_it == d->entries.end()) {
                int idx = o_it - d->entries.begin();
                beginInsertRows(QModelIndex(), idx,idx);
                o_it = d->entries.insert(o_it, *n_it);
                endInsertRows();

                o_it++;
                n_it++;
            }
            else if (n_it == entries.end()) {
                int idx = o_it - d->entries.begin();
                beginRemoveRows(QModelIndex(), idx,idx);
                o_it = d->entries.erase(o_it);
                endRemoveRows();
            }
            else if ((*n_it).fileName() < (*o_it).fileName()) {
                int idx = o_it - d->entries.begin();
                beginInsertRows(QModelIndex(), idx,idx);
                o_it = d->entries.insert(o_it, *n_it);
                endInsertRows();

                o_it++;
                n_it++;
            }
            else if ((*n_it).fileName() > (*o_it).fileName()) {
                int idx = o_it - d->entries.begin();
                beginRemoveRows(QModelIndex(), idx,idx);
                o_it = d->entries.erase(o_it);
                endRemoveRows();
            }
            else {
                *(o_it) = *(n_it);

                //int idx = o_it - d->entries.begin();
                //emit dataChanged(index(idx,0), index(idx,columnCount()-1));
            }
        }
    }
    //reset();

    if (d->watcher.files().count())
        d->watcher.removePaths( d->watcher.files() );
    if (d->watcher.directories().count())
        d->watcher.removePaths( d->watcher.directories() );
    
    d->watcher.addPath(rootPath()+path());
}

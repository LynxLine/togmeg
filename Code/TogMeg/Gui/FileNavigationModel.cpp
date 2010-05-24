
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
        else if ( i.column() == ColLinks ) {
            //int linkCount = 0;
            //return linkCount;
            return QVariant();
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
    else if (role == Qt::TextAlignmentRole && i.column() == ColLinks) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
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
    d->entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
    d->entries += dir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
    if (!path().isEmpty())
        d->entries.prepend(QFileInfo(rootPath()+".."));
    reset();

    if (d->watcher.files().count())
        d->watcher.removePaths( d->watcher.files() );
    if (d->watcher.directories().count())
        d->watcher.removePaths( d->watcher.directories() );
    
    d->watcher.addPath(rootPath()+path());
}

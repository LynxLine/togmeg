//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "tasklistmodel.h"

class TaskListModel::Private
{
public:
};

/*!
 Crestes TaskListModel
 */
TaskListModel::TaskListModel(QObject * parent)
:QAbstractItemModel(parent)
{
    d = new Private;

    /*
    d->root = new CatalogItem(this);

    QTime time = QTime::currentTime();


    //for (int i=0;i<100;i++)
    {
        QFile catalogFile(app::storagePath()+"catalog.xml");
        if (catalogFile.open(QIODevice::ReadOnly)) {
            QDomDocument xml;
            xml.setContent( &catalogFile );
            catalogFile.close();

            xml >> this;
        }
    }
    qDebug() << "time for loading xml:" << time.msecsTo(QTime::currentTime());

    QDir storageDir(app::storagePath());
    QStringList dirs = storageDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString dirName, dirs) {
        QString taskPath = app::storagePath()+dirName;
        DataContainer * container = DataContainerFactory::resourceContainer( taskPath );
        if ( !container ) continue;
        d->containers << container;
    }
    */
}

/*!
 Deletes the object.
 */
TaskListModel::~TaskListModel()
{
    qDebug() << "~TaskListModel";
    delete d;
}

/*!
 \reimp for Catalog-like Model.
 */
QModelIndex TaskListModel::parent(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return QModelIndex();
    /*
    CatalogItem * child = static_cast<CatalogItem*>(index.internalPointer());
    CatalogItem * parent = child->parent();

    if ( !parent ) return QModelIndex();
    return createIndex(parent->row(), 0, parent);
    */
    return QModelIndex();
}

/*!
 \reimp for Catalog-like Model.
 */
Qt::ItemFlags TaskListModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    //CatalogItem * item = static_cast<CatalogItem*>(index.internalPointer());
    //if (item == root() ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/*!
 It uses data() of item.
 Do not reimplemnt this.
 */
QVariant TaskListModel::data(const QModelIndex &index, int role) const 
{
    if ( !index.isValid() ) return QVariant();
    return QVariant();

    //CatalogItem * item = static_cast<CatalogItem *>(index.internalPointer());
    //return item->data(role);
}

bool TaskListModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if ( !index.isValid() ) return false;
    return false;
    
    //CatalogItem * item = static_cast<CatalogItem *>(index.internalPointer());
    //return item->setData(value, role);
}

/*!
 \reimp for Catalog-like Model.
 */
QModelIndex TaskListModel::index(int row, int column, const QModelIndex &parent) const 
{
    //CatalogItem * p;
    if ( !parent.isValid() ) {
        //if (row == 0) return createIndex(row, column, root());
        return QModelIndex();
    }
    //else p = static_cast<CatalogItem *>(parent.internalPointer());
    return QModelIndex();

    //CatalogItem * child = p->child(row);
    //if ( !child ) return QModelIndex();

    //return createIndex(row, column, child);
}

/*!
 \reimp for Catalog-like Model.
 */
int TaskListModel::rowCount(const QModelIndex &parent) const 
{
    return 0;

    /*
    CatalogItem * p;

    if (!parent.isValid()) return 1; //root only
    else p = static_cast<CatalogItem *>(parent.internalPointer());

    return p->count();
    */
}

int TaskListModel::columnCount(const QModelIndex &) const 
{
	return 1;
}

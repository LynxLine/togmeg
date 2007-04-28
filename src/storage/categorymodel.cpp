//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtGui>
#include <QtCore>

#include "crammero.h"
#include "categorymodel.h"

/*!
 Creates new root item associated with \a model.
 */
CategoryItem::CategoryItem(QAbstractItemModel * model)
:_parent(0L), _model(model), _level(0)
{
    setText( "Library" );
}

/*!
 Creates new item and append it to parent.
 */
CategoryItem::CategoryItem(QString id, CategoryItem * parent)
:_parent(parent), _id(id)
{
    Q_ASSERT( parent );
    parent->append( this );
}

CategoryItem::CategoryItem(QString id, int index, CategoryItem * parent)
:_parent(parent), _id(id)
{
    Q_ASSERT( parent );
    parent->insert( index, this );
}

/*!
 Deletes all children of the item.
 */
CategoryItem::~CategoryItem() 
{
    qDeleteAll(_children);;
}

/*!
 Returns model associated with the item.
 */
QAbstractItemModel * CategoryItem::model()
{
    return _model;
}

/*!
 Returns current row relative to parent.
 */
int CategoryItem::row()
{
    if( !parent() ) return 0;
    return parent()->_children.indexOf( this );
}

/*!
 Returns parent item.
 */
CategoryItem * CategoryItem::parent()
{
    return _parent;
}

QString CategoryItem::id()
{
    return _id;
}

QString CategoryItem::compositeId()
{
    QString id = _id;
    CategoryItem * p = _parent;
    while(p) {
        if ( !p->id().isEmpty() )
            id = p->id() +"/" +id;
        p = p->parent();
    }
    return id;
}

QString CategoryItem::text()
{
    return _text;
}

void CategoryItem::setText(QString t)
{
    _text = t;
}

QPixmap CategoryItem::icon()
{
    return _icon;
}

void CategoryItem::setIcon(QPixmap icon)
{
    _icon = icon;
}

bool CategoryItem::isExpanded()
{
    return _expanded;
}

void CategoryItem::setExpanded(bool f)
{
    _expanded = f;
}


QVariant CategoryItem::data(int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) return text();
    if (role == Qt::DecorationRole) return icon();
    return QVariant();
}

bool CategoryItem::setData(QVariant value, int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        setText( value.toString() );
        return true;
    }
    return false;
}

/*!
 Return child at \a index.
 */
CategoryItem * CategoryItem::child(int index)
{
    if ( index >= count() ) return 0L;
    return _children.at(index);
}

/*!
 Return children list.
 */
QList<CategoryItem *> & CategoryItem::children()
{
    return _children;
}

/*!
 Append a \a item to children.
 Do not use the function, use the constructor 
 which adds itself to parent.
 */
void CategoryItem::append(CategoryItem * item) 
{
    _children.append(item);
    item->_level = _level+1;
    item->_model = _model;
}

void CategoryItem::insert(int index, CategoryItem * item) 
{
    _children.insert(index, item);
    item->_level = _level+1;
    item->_model = _model;
}

/*!
 Removes child at \a index.
 */
void CategoryItem::remove(int index) 
{
    _children.removeAt(index);
}

/*!
 Returns number of children.
 */
int CategoryItem::count()
{
    return _children.count();
}

/*!
 Returns level of the item.
 */
int CategoryItem::level()
{
    return _level;
}

class CategoryModel::Private
{
public:
    CategoryItem * root;

};

/*!
 Crestes CategoryModel
 */
CategoryModel::CategoryModel(QObject * parent)
:QAbstractItemModel(parent)
{
    d = new Private;
    d->root = new CategoryItem(this);
    d->root->setIcon( QPixmap(":/images/icons/library-icon-20x20.png") );

    QTime time = QTime::currentTime();

    QFile catalogFile(app::storagePath()+"catalog.xml");
    if (catalogFile.open(QIODevice::ReadOnly)) {
        QDomDocument xml;
        xml.setContent( &catalogFile );
        catalogFile.close();

        xml >> this;
    }

    qDebug() << " T loading catalog.xml:" << time.msecsTo(QTime::currentTime());
}

/*!
 Deletes the object.
 */
CategoryModel::~CategoryModel()
{
    qDebug() << "~CategoryModel";

    QFile catalogFile(app::storagePath()+"catalog.xml");
    if (catalogFile.open(QIODevice::WriteOnly)) {
        QDomDocument xml("catalogxml");
        xml << this;
        
        catalogFile.write(xml.toByteArray());
        catalogFile.close();
    }

    delete d;
}

/*!
 Returns root item
 */
CategoryItem * CategoryModel::root() const
{
    return d->root;
}

/*!
 Returns item of index
 */
CategoryItem * CategoryModel::item(const QModelIndex &index)
{
    if ( !index.isValid() ) return 0L;
    return static_cast<CategoryItem*>(index.internalPointer());
}

/*!
 \reimp for Category-like Model.
 */
QModelIndex CategoryModel::parent(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return QModelIndex();
    
    CategoryItem * child = static_cast<CategoryItem*>(index.internalPointer());
    CategoryItem * parent = child->parent();

    if ( !parent ) return QModelIndex();
    return createIndex(parent->row(), 0, parent);
}

/*!
 \reimp for Category-like Model.
 */
Qt::ItemFlags CategoryModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    CategoryItem * item = static_cast<CategoryItem*>(index.internalPointer());
    if (item == root() ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/*!
 It uses data() of item.
 Do not reimplemnt this.
 */
QVariant CategoryModel::data(const QModelIndex &index, int role) const 
{
    if ( !index.isValid() ) return QVariant();
    
    CategoryItem * item = static_cast<CategoryItem *>(index.internalPointer());
    return item->data(role);
}

bool CategoryModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if ( !index.isValid() ) return false;
    
    CategoryItem * item = static_cast<CategoryItem *>(index.internalPointer());
    return item->setData(value, role);
}

/*!
 \reimp for Category-like Model.
 */
QModelIndex CategoryModel::index(int row, int column, const QModelIndex &parent) const 
{
    CategoryItem * p;
    if ( !parent.isValid() ) {
        if (row == 0) return createIndex(row, column, root());
        return QModelIndex();
    }
    else p = static_cast<CategoryItem *>(parent.internalPointer());

    CategoryItem * child = p->child(row);
    if ( !child ) return QModelIndex();

    return createIndex(row, column, child);
}

QModelIndex CategoryModel::indexOf(CategoryItem * item) 
{
    QModelIndex index;
    index = createIndex(item->row(), 0, item);
    return index;
}

/*!
 \reimp for Category-like Model.
 */
int CategoryModel::rowCount(const QModelIndex &parent) const 
{
    CategoryItem * p;

    if (!parent.isValid()) return 1; //root only
    else p = static_cast<CategoryItem *>(parent.internalPointer());

    return p->count();
}

int CategoryModel::columnCount(const QModelIndex &) const 
{
	return 1;
}

CategoryItem * CategoryModel::createItem(QString name, CategoryItem * parent)
{
    if ( !parent ) return 0L;

    QModelIndex index = createIndex(parent->row(), 0, parent);

    beginInsertRows(index, parent->count(), parent->count());
    CategoryItem * item = new CategoryItem(app::uniqueId(), parent);
    item->setIcon( QPixmap(":/images/icons/category-icon-20x20.png") );
    item->setText(name);
    endInsertRows();

    return item;
}

/*!
 Removes \a item associated with the model.
 */
void CategoryModel::removeItem(CategoryItem * item)
{
    CategoryItem * parent = item->parent();
    if ( !parent ) return;

    int i = parent->children().indexOf( item );
    QModelIndex index = createIndex(parent->row(), 0, parent);

    beginRemoveRows(index, i, i);
    parent->remove(i);
    endRemoveRows();
}

/*!
 Updates the \a item in tree.
 */
void CategoryModel::updateItem(CategoryItem * item)
{
    QModelIndex index = createIndex( item->row(), 0, item );
    emit dataChanged(index, index);
}

QDomDocument& operator>>(QDomDocument & doc, CategoryModel * categoryModel)
{
    CategoryItem * item = categoryModel->root();    
    QDomNode node = doc.documentElement();
    node >> item;
    return doc;
}

QDomNode& operator>>(QDomNode & node, CategoryItem * item)
{
    QDomElement el = node.toElement();
    item->setText(el.attribute("name"));
    item->setExpanded(el.attribute("expanded") == "yes");

    QDomNode n = node.firstChild();
    while (!n.isNull()) {
        QDomElement el = n.toElement();

        CategoryItem * child = new CategoryItem(el.attribute("id"), item);
        child->setIcon( QPixmap(":/images/icons/category-icon-20x20.png") );

        n >> child;
        n = n.nextSibling();
    }

    return node;
}

QDomDocument& operator<<(QDomDocument & doc, CategoryModel * categoryModel)
{
    doc << categoryModel->root();
    return doc;
}

QDomNode& operator<<(QDomNode & node, CategoryItem * item)
{
    QDomElement child = node.ownerDocument().createElement("category");
    child.setAttribute("name", item->text());
    child.setAttribute("id", item->id());
    child.setAttribute("expanded", item->isExpanded() ? "yes" : "no");
    node.appendChild(child);

    foreach(CategoryItem * item, item->children()) {
        child << item;
    }

    return node;
}

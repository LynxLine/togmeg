//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "catalogmodel.h"

/*!
 Creates new root item associated with \a model.
 */
CatalogItem::CatalogItem(QAbstractItemModel * model)
:_parent(0L), _model(model), _level(0)
{
    setText("CATALOG");
}

/*!
 Creates new item and append it to parent.
 */
CatalogItem::CatalogItem(CatalogItem * parent)
:_parent(parent)
{
    Q_ASSERT( parent );
    parent->append( this );
}

/*!
 Deletes all children of the item.
 */
CatalogItem::~CatalogItem() 
{
    qDeleteAll(_children);;
}

/*!
 Returns model associated with the item.
 */
QAbstractItemModel * CatalogItem::model()
{
    return _model;
}

/*!
 Returns current row relative to parent.
 */
int CatalogItem::row()
{
    if( !parent() ) return 0;
    return parent()->_children.indexOf( this );
}

/*!
 Returns parent item.
 */
CatalogItem * CatalogItem::parent()
{
    return _parent;
}

QString CatalogItem::text()
{
    return _text;
}

void CatalogItem::setText(QString t)
{
    _text = t;
}

QVariant CatalogItem::data(int column, int role)
{
    Q_UNUSED(column);
    if (role == Qt::DisplayRole) return text();
    return QVariant();
}

/*!
 Return child at \a index.
 */
CatalogItem * CatalogItem::child(int index)
{
    if ( index >= count() ) return 0L;
    return _children.at(index);
}

/*!
 Return children list.
 */
QList<CatalogItem *> & CatalogItem::children()
{
    return _children;
}

/*!
 Append a \a item to children.
 Do not use the function, use the constructor 
 which adds itself to parent.
 */
void CatalogItem::append(CatalogItem * item) 
{
    _children.append(item);
    item->_level = _level+1;
    item->_model = _model;
}

/*!
 Removes child at \a index.
 */
void CatalogItem::remove(int index) 
{
    _children.removeAt(index);
}

/*!
 Returns number of children.
 */
int CatalogItem::count()
{
    return _children.count();
}

class CatalogModel::Private
{
public:
    CatalogItem * root;
};

/*!
 Crestes CatalogModel
 */
CatalogModel::CatalogModel(QObject * parent)
:QAbstractItemModel(parent)
{
    d = new Private;
    d->root = new CatalogItem(this);

    QFile catalogFile("catalog.xml");
    if (catalogFile.open(QIODevice::ReadOnly)) {
        QDomDocument xml;
        xml.setContent( &catalogFile );
        catalogFile.close();

        xml >> this;
    }
}

/*!
 Deletes the object.
 */
CatalogModel::~CatalogModel()
{
    QFile catalogFile("catalog.xml");
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
CatalogItem * CatalogModel::root() const
{
    return d->root;
}

/*!
 \reimp for Catalog-like Model.
 */
QModelIndex CatalogModel::parent(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return QModelIndex();
    
    CatalogItem * child = static_cast<CatalogItem*>(index.internalPointer());
    CatalogItem * parent = child->parent();

    if ( !parent ) return QModelIndex();
    return createIndex(parent->row(), 0, parent);
}

/*!
 \reimp for Catalog-like Model.
 */
Qt::ItemFlags CatalogModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/*!
 It uses data() of item.
 Do not reimplemnt this.
 */
QVariant CatalogModel::data(const QModelIndex &index, int role) const 
{
    if ( !index.isValid() ) return QVariant();
    
    CatalogItem * item = static_cast<CatalogItem *>(index.internalPointer());
    return item->data( index.column(), role);
}

/*!
 \reimp for Catalog-like Model.
 */
QModelIndex CatalogModel::index(int row, int column, const QModelIndex &parent) const 
{
    CatalogItem * p;
    if ( !parent.isValid() ) {
        if (row == 0) return createIndex(row, column, root());
        return QModelIndex();
    }
    else p = static_cast<CatalogItem *>(parent.internalPointer());

    CatalogItem * child = p->child(row);
    if ( !child ) return QModelIndex();

    return createIndex(row, column, child);
}

/*!
 \reimp for Catalog-like Model.
 */
int CatalogModel::rowCount(const QModelIndex &parent) const 
{
    CatalogItem * p;

    if (!parent.isValid()) return 1; //root only
    else p = static_cast<CatalogItem *>(parent.internalPointer());

    return p->count();
}

int CatalogModel::columnCount(const QModelIndex &) const 
{
	return 1;
}

QDomDocument& operator>>(QDomDocument & doc, CatalogModel * catalogModel)
{
    CatalogItem * item = catalogModel->root();    
    QDomNode node = doc.documentElement();
    node >> item;
    return doc;
}

QDomNode& operator>>(QDomNode & node, CatalogItem * item)
{
    QDomElement el = node.toElement();
    item->setText(el.attribute("name"));

    QDomNode n = node.firstChild();
    while (!n.isNull()) {
        CatalogItem * child = new CatalogItem(item);
        n >> child;
        n = n.nextSibling();
    }

    return node;
}

QDomDocument& operator<<(QDomDocument & doc, CatalogModel * catalogModel)
{
    doc << catalogModel->root();
    return doc;
}

QDomNode& operator<<(QDomNode & node, CatalogItem * item)
{
    QDomElement child = node.ownerDocument().createElement("category");
    child.setAttribute("name", item->text());
    node.appendChild(child);

    foreach(CatalogItem * item, item->children()) {
        child << item;
    }

    return node;
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "catalogview.h"
#include "catalogmodel.h"
#include "mainwindow.h"

class CatalogView::Private
{
public:
    QPointer<QMenu> contextMenu;
};

CatalogView::CatalogView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    setRootIsDecorated(false);
    header()->hide();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( _action("category/add_category") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( _action("category/remove") );
    
    connect( _action("category/add_category"), SIGNAL(triggered()), 
             this, SLOT(addSubCategory()));
    connect( _action("category/remove"), SIGNAL(triggered()), 
             this, SLOT(removeCategory()));

    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(saveExpandState(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(saveCollapseState(const QModelIndex &)));
}

CatalogView::~CatalogView()
{
    qDebug() << "~CatalogView";
    delete d;
}

void CatalogView::setModel(QAbstractItemModel * model)
{
    QTreeView::setModel(model);

    CatalogModel * catalogModel = (CatalogModel *)model;
    loadExpandState(catalogModel->root());
}

/*
void CatalogView::drawBranches(QPainter *, const QRect &, const QModelIndex &) const
{
}
*/

/*
void CatalogView::drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}
*/

void CatalogView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( mapToGlobal(pos) );
}

void CatalogView::loadExpandState(CatalogItem * item)
{
    if (item->isExpanded()) {
        CatalogModel * catalogModel = (CatalogModel *)model();
        setExpanded(catalogModel->indexOf(item), true);
    }

    foreach(CatalogItem * child, item->children()) {
        loadExpandState(child);
    }
}

void CatalogView::saveExpandState(const QModelIndex & index)
{
    CatalogModel * catalogModel = (CatalogModel *)model();
    CatalogItem * item = catalogModel->item(index);
    if (!item) return;

    item->setExpanded(true);
}

void CatalogView::saveCollapseState(const QModelIndex & index)
{
    CatalogModel * catalogModel = (CatalogModel *)model();
    CatalogItem * item = catalogModel->item(index);
    if (!item) return;

    item->setExpanded(false);
}

void CatalogView::addSubCategory()
{
    QModelIndex parentIndex = currentIndex();
    CatalogModel * catalogModel = (CatalogModel *)model();
    CatalogItem * parent = catalogModel->item(parentIndex);
    if (!parent) return;

    CatalogItem * item = catalogModel->createItem(tr("New Category"), parent);

    setExpanded(parentIndex, true);
    setCurrentIndex( catalogModel->indexOf(item) );
    edit( catalogModel->indexOf(item) );
}

void CatalogView::removeCategory()
{
    QModelIndex index = currentIndex();
    CatalogModel * catalogModel = (CatalogModel *)model();
    CatalogItem * item = catalogModel->item(index);
    if (item==catalogModel->root()) return;
    if (!item) return;

    catalogModel->removeItem(item);
    scrollTo(currentIndex());
}

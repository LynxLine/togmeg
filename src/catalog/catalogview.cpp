//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "catalogview.h"
#include "categorymodel.h"
#include "mainwindow.h"

class CategoryView::Private
{
public:
    QPointer<QMenu> contextMenu;
};

CategoryView::CategoryView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    setRootIsDecorated(false);
    setFrameStyle(QFrame::NoFrame);
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

CategoryView::~CategoryView()
{
    qDebug() << "~CategoryView";
    delete d;
}

void CategoryView::setModel(QAbstractItemModel * model)
{
    QTreeView::setModel(model);

    CategoryModel * CategoryModel = (CategoryModel *)model;
    loadExpandState(CategoryModel->root());
}

/*
void CategoryView::drawBranches(QPainter *, const QRect &, const QModelIndex &) const
{
}
*/

/*
void CategoryView::drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}
*/

void CategoryView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( mapToGlobal(pos) );
}

void CategoryView::loadExpandState(CategoryItem * item)
{
    if (item->isExpanded()) {
        CategoryModel * CategoryModel = (CategoryModel *)model();
        setExpanded(CategoryModel->indexOf(item), true);
    }

    foreach(CategoryItem * child, item->children()) {
        loadExpandState(child);
    }
}

void CategoryView::saveExpandState(const QModelIndex & index)
{
    CategoryModel * CategoryModel = (CategoryModel *)model();
    CategoryItem * item = CategoryModel->item(index);
    if (!item) return;

    item->setExpanded(true);
}

void CategoryView::saveCollapseState(const QModelIndex & index)
{
    CategoryModel * CategoryModel = (CategoryModel *)model();
    CategoryItem * item = CategoryModel->item(index);
    if (!item) return;

    item->setExpanded(false);
}

void CategoryView::addSubCategory()
{
    QModelIndex parentIndex = currentIndex();
    CategoryModel * CategoryModel = (CategoryModel *)model();
    CategoryItem * parent = CategoryModel->item(parentIndex);
    if (!parent) return;

    CategoryItem * item = CategoryModel->createItem(tr("New Category"), parent);

    setExpanded(parentIndex, true);
    setCurrentIndex( CategoryModel->indexOf(item) );
    edit( CategoryModel->indexOf(item) );
}

void CategoryView::removeCategory()
{
    QModelIndex index = currentIndex();
    CategoryModel * CategoryModel = (CategoryModel *)model();
    CategoryItem * item = CategoryModel->item(index);
    if (item==CategoryModel->root()) return;
    if (!item) return;

    CategoryModel->removeItem(item);
    scrollTo(currentIndex());
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "categoryview.h"
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
    setEditTriggers(editTriggers() | QAbstractItemView::SelectedClicked);
    setFrameStyle(QFrame::NoFrame);
    setUniformRowHeights(true);
    setAutoFillBackground(true);
    setAnimated(true);
    header()->hide();

    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, "#E9EFF9");
        setPalette(palette);
    }

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("&Create new category"), this, SLOT(addNewCategory()));
    d->contextMenu->addAction( tr("Create &sub-category"), this, SLOT(addNewCategory()));
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("Re&name"), this, SLOT(renameCategory()), QKeySequence(Qt::Key_F2));
    d->contextMenu->addAction( tr("&Remove"), this, SLOT(removeCategory()));
    //d->contextMenu->addSeparator();
    //d->contextMenu->addAction( tr("Properties"));
    
    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(saveExpandState(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(saveCollapseState(const QModelIndex &)));

    connect(this, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(activateItem(const QModelIndex &)));
    connect(this, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(activateItem(const QModelIndex &)));
}

CategoryView::~CategoryView()
{
    qDebug() << "~CategoryView";
    delete d;
}

void CategoryView::setModel(QAbstractItemModel * model)
{
    QTreeView::setModel(model);

    CategoryModel * categoryModel = (CategoryModel *)model;
    loadExpandState(categoryModel->root());
}

/*
void CategoryView::drawBranches(QPainter *, const QRect &, const QModelIndex &) const
{
}
*/


void CategoryView::drawRow(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    opt.showDecorationSelected = false;
    if ( index == currentIndex() ) {
        opt.font.setBold(true);
    }
    QTreeView::drawRow(painter, opt, index);
}


void CategoryView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void CategoryView::loadExpandState(CategoryItem * item)
{
    if (item->isExpanded()) {
        CategoryModel * categoryModel = (CategoryModel *)model();
        setExpanded(categoryModel->indexOf(item), true);
    }

    foreach(CategoryItem * child, item->children()) {
        loadExpandState(child);
    }
}

void CategoryView::saveExpandState(const QModelIndex & index)
{
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * item = categoryModel->item(index);
    if (!item) return;

    item->setExpanded(true);
}

void CategoryView::saveCollapseState(const QModelIndex & index)
{
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * item = categoryModel->item(index);
    if (!item) return;

    item->setExpanded(false);
}

void CategoryView::addNewCategory()
{
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * parent = categoryModel->root();
    if (!parent) return;

    CategoryItem * item = categoryModel->createItem(tr("New Category"), parent);

    setExpanded( categoryModel->indexOf(parent), true);
    setCurrentIndex( categoryModel->indexOf(item) );
    emit categoryActivated(item->compositeId());
    edit( categoryModel->indexOf(item) );
}

void CategoryView::addSubCategory()
{
    QModelIndex parentIndex = currentIndex();
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * parent = categoryModel->item(parentIndex);
    if (!parent) return;

    CategoryItem * item = categoryModel->createItem(tr("New Category"), parent);

    setExpanded(parentIndex, true);
    setCurrentIndex( categoryModel->indexOf(item) );
    emit categoryActivated(item->compositeId());
    edit( categoryModel->indexOf(item) );
}

void CategoryView::renameCategory()
{
    QModelIndex index = currentIndex();
    scrollTo( index );
    edit( index );
}

void CategoryView::removeCategory()
{
    QModelIndex index = currentIndex();
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * item = categoryModel->item(index);
    if (item==categoryModel->root()) return;
    if (!item) return;

    categoryModel->removeItem(item);
    scrollTo(currentIndex());
}

void CategoryView::activateItem(const QModelIndex & index)
{
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * item = categoryModel->item(index);
    if (!item) return;

    QString category;
    if (item==categoryModel->root()) {
        emit categoryActivated(QString::null);
        return;
    }

    emit categoryActivated(item->compositeId());
}

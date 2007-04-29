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
    setFrameStyle(QFrame::NoFrame);
    setUniformRowHeights(true);
    setAutoFillBackground(true);
    setItemDelegate(new CategoryItemDelegate(this));
    setIndentation(20);
    setAnimated(false);
    header()->hide();

    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, "#E9EFF9");
        palette.setColor(QPalette::Highlight, "#B5D5FF");
        palette.setColor(QPalette::HighlightedText, "#000000");
        setPalette(palette);
    }

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("&Create new category"), this, SLOT(addNewCategory()));
    d->contextMenu->addAction( tr("Create &sub-category"), this, SLOT(addSubCategory()));
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
    //QTreeView::drawRow(painter, option, index);

    
    const QModelIndex parent = index.parent();
    const QModelIndex current = currentIndex();

    QStyleOptionViewItemV2 opt = option;
    const int y = option.rect.y();

    opt.showDecorationSelected = false;

    int width = header()->sectionSize(0);
    int height = option.rect.height();

    opt.rect.setRect(0, y, width, height);
    painter->fillRect(opt.rect, palette().brush(QPalette::Base));

    QModelIndex modelIndex = model()->index( index.row(), 0, parent );
    if (!modelIndex.isValid()) {
        return;
    }

    if (selectionModel()->isSelected(modelIndex)) {
        opt.state |= QStyle::State_Selected;
        opt.font.setBold(true);

        if ( opt.state & QStyle::State_Active ) {
            QLinearGradient linearGradient(QPointF(0, y), QPointF(0, y+height));
            linearGradient.setColorAt(0, "#5F9CFE");
            linearGradient.setColorAt(1, "#1E61CD");

            painter->fillRect(opt.rect, linearGradient);
            opt.palette.setBrush(QPalette::Active, QPalette::Highlight, linearGradient);
        }
        else {
            QLinearGradient linearGradient(QPointF(0, y), QPointF(0, y+height));
            linearGradient.setColorAt(0, "#A8B7CE");
            linearGradient.setColorAt(1, "#939CAB");

            painter->fillRect(opt.rect, linearGradient);
            opt.palette.setBrush(QPalette::Inactive, QPalette::Highlight, linearGradient);
        }

        opt.palette.setColor(QPalette::HighlightedText, "#FFFFFF");
    }
    
    CategoryModel * categoryModel = (CategoryModel *)model();
    CategoryItem * item = categoryModel->item(index);
    int x = item->level() * indentation();

    opt.rect.setRect(x, y, width - x, height);
    
    QRect branches(0, y, x, height);
    drawBranches(painter, branches, index);
    
    itemDelegate()->paint(painter, opt, modelIndex);
    
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

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "catalogview.h"
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
	d->contextMenu->addAction( _action("category/remove_category") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( _action("category/add_task") );
    d->contextMenu->addAction( _action("category/remove_task") );
    
}

CatalogView::~CatalogView()
{
    delete d;
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

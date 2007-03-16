//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "catalogwidget.h"

class CatalogWidget::Private
{
public:
    QTreeView * testCatalogView;
    QListView * testContentView;
};

/*!
 Crestes CatalogWidget
 */
CatalogWidget::CatalogWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setMargin(10);
    layout->setSpacing(5);
    setLayout(layout);

    d->testCatalogView = new QTreeView(this);
    layout->addWidget( d->testCatalogView );

    {
        d->testCatalogView->setFixedWidth(200);
    }

    d->testContentView = new QListView(this);
    layout->addWidget( d->testContentView );
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "categoryview.h"
#include "categorymodel.h"
#include "catalogwidget.h"
#include "tasklistview.h"

#include "mainwindow.h"

class CatalogWidget::Private
{
public:
    CategoryView * catalogView;
    TaskListView * taskListView;
    CategoryModel * categoryModel;
};

/*!
 Crestes CatalogWidget
 */
CatalogWidget::CatalogWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QGridLayout * layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(1);
    setLayout(layout);

    //category area
    QVBoxLayout * categoryLayout = new QVBoxLayout;
    categoryLayout->setMargin(0);
    categoryLayout->setSpacing(5);
    layout->addLayout(categoryLayout, 0, 0);

    d->categoryModel = new CategoryModel(this);
    d->catalogView = new CategoryView(this);

    {
        d->catalogView->setFixedWidth(200);
        d->catalogView->setModel( d->categoryModel );
        d->catalogView->setCurrentIndex( d->categoryModel->index(0,0) );
    }

    categoryLayout->addWidget( d->catalogView );

    //task list area
    QVBoxLayout * taskLayout = new QVBoxLayout;
    taskLayout->setMargin(0);
    taskLayout->setSpacing(5);
    layout->addLayout(taskLayout, 0, 1);

    d->taskListView = new TaskListView(this);
    taskLayout->addWidget( d->taskListView );

    //bottom footer
    QHBoxLayout * manageCatalogLayout = new QHBoxLayout;
    manageCatalogLayout->setMargin(0);
    manageCatalogLayout->setSpacing(5);
    layout->addLayout( manageCatalogLayout, 1, 0, 1, 2 );

    QToolButton * addCategory = new QToolButton;
    QToolButton * remCategory = new QToolButton;

    addCategory->setText("+Category");
    remCategory->setText("-Category");

    connect(addCategory, SIGNAL(clicked()), d->catalogView, SLOT(addSubCategory()));
    connect(remCategory, SIGNAL(clicked()), d->catalogView, SLOT(removeCategory()));

    manageCatalogLayout->addWidget( addCategory );
    manageCatalogLayout->addWidget( remCategory );
    manageCatalogLayout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum) );
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

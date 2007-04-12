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
    QHBoxLayout * footerAreaLayout = new QHBoxLayout;
    footerAreaLayout->setMargin(0);
    footerAreaLayout->setSpacing(5);
    layout->addLayout( footerAreaLayout, 1, 0, 1, 2 );

    QToolButton * addCategory = new QToolButton;
    QToolButton * remCategory = new QToolButton;

    addCategory->setText("+Category");
    remCategory->setText("-Category");

    connect(addCategory, SIGNAL(clicked()), d->catalogView, SLOT(addSubCategory()));
    connect(remCategory, SIGNAL(clicked()), d->catalogView, SLOT(removeCategory()));

    QToolButton * addStudy = new QToolButton;
    QToolButton * remStudy = new QToolButton;

    addStudy->setText("+Study");
    remStudy->setText("-Study");

    connect(addStudy, SIGNAL(clicked()), d->taskListView, SLOT(addNewStudy()));
    connect(remStudy, SIGNAL(clicked()), d->taskListView, SLOT(removeStudy()));

    footerAreaLayout->addWidget( addCategory );
    footerAreaLayout->addWidget( remCategory );
    footerAreaLayout->addItem(new QSpacerItem(30,10, QSizePolicy::Fixed, QSizePolicy::Minimum) );
    footerAreaLayout->addWidget( addStudy );
    footerAreaLayout->addWidget( remStudy );
    footerAreaLayout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum) );
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "catalogview.h"
#include "catalogmodel.h"
#include "catalogwidget.h"

#include "mainwindow.h"

class CatalogWidget::Private
{
public:
    CatalogView * taskCatalogView;
    QListView * taskContentView;
    CatalogModel * catalogModel;
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

    QVBoxLayout * catalogLayout = new QVBoxLayout;
    catalogLayout->setMargin(0);
    catalogLayout->setSpacing(5);
    layout->addLayout(catalogLayout);

    d->catalogModel = new CatalogModel(this);
    d->taskCatalogView = new CatalogView(this);

    {
        d->taskCatalogView->setFixedWidth(200);
        d->taskCatalogView->setModel( d->catalogModel );
        d->taskCatalogView->setCurrentIndex( d->catalogModel->index(0,0) );
    }

    catalogLayout->addWidget( d->taskCatalogView );

    QHBoxLayout * manageCatalogLayout = new QHBoxLayout;
    manageCatalogLayout->setMargin(0);
    manageCatalogLayout->setSpacing(5);
    catalogLayout->addLayout( manageCatalogLayout );

    QPushButton * addCategory = new QPushButton("+");
    QPushButton * remCategory = new QPushButton("-");

    manageCatalogLayout->addWidget( addCategory );
    manageCatalogLayout->addWidget( remCategory );
    //manageCatalogLayout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum) );

    QVBoxLayout * taskLayout = new QVBoxLayout;
    taskLayout->setMargin(0);
    taskLayout->setSpacing(5);
    layout->addLayout(taskLayout);

    QHBoxLayout * manageTaskLayout = new QHBoxLayout;
    manageTaskLayout->setMargin(0);
    manageTaskLayout->setSpacing(5);
    taskLayout->addLayout( manageTaskLayout );

    QPushButton * demoTask = new QPushButton("Demo");
    QPushButton * studyTask = new QPushButton("Study");
    QPushButton * examineTask = new QPushButton("Examine");

    connect(demoTask, SIGNAL(clicked()), _action("app/demo"), SLOT(trigger()));
    connect(studyTask, SIGNAL(clicked()), _action("app/study"), SLOT(trigger()));
    connect(examineTask, SIGNAL(clicked()), _action("app/exam"), SLOT(trigger()));

    //manageTaskLayout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum) );
    manageTaskLayout->addWidget( demoTask );
    manageTaskLayout->addWidget( studyTask );
    manageTaskLayout->addWidget( examineTask );

    d->taskContentView = new QListView(this);
    taskLayout->addWidget( d->taskContentView );
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

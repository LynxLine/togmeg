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
    CatalogView * testCatalogView;
    QListView * testContentView;
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
    d->testCatalogView = new CatalogView(this);

    {
        d->testCatalogView->setFixedWidth(200);
        d->testCatalogView->setModel( d->catalogModel );
        d->testCatalogView->expand( d->catalogModel->index(0,0) );
        d->testCatalogView->setRootIsDecorated(false);
        d->testCatalogView->header()->hide();
        d->testCatalogView->setCurrentIndex( d->catalogModel->index(0,0) );
    }

    catalogLayout->addWidget( d->testCatalogView );

    QHBoxLayout * manageCatalogLayout = new QHBoxLayout;
    manageCatalogLayout->setMargin(0);
    manageCatalogLayout->setSpacing(5);
    catalogLayout->addLayout( manageCatalogLayout );

    QPushButton * addCategory = new QPushButton("+");
    QPushButton * remCategory = new QPushButton("-");

    manageCatalogLayout->addWidget( addCategory );
    manageCatalogLayout->addWidget( remCategory );
    //manageCatalogLayout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum) );

    QVBoxLayout * testLayout = new QVBoxLayout;
    testLayout->setMargin(0);
    testLayout->setSpacing(5);
    layout->addLayout(testLayout);

    QHBoxLayout * manageTestLayout = new QHBoxLayout;
    manageTestLayout->setMargin(0);
    manageTestLayout->setSpacing(5);
    testLayout->addLayout( manageTestLayout );

    QPushButton * demoTest = new QPushButton("Demo");
    QPushButton * studyTest = new QPushButton("Study");
    QPushButton * examineTest = new QPushButton("Examine");

    connect(demoTest, SIGNAL(clicked()), _action("ad/demo"), SLOT(trigger()));
    connect(studyTest, SIGNAL(clicked()), _action("ad/study"), SLOT(trigger()));
    connect(examineTest, SIGNAL(clicked()), _action("ad/exam"), SLOT(trigger()));

    //manageTestLayout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum) );
    manageTestLayout->addWidget( demoTest );
    manageTestLayout->addWidget( studyTest );
    manageTestLayout->addWidget( examineTest );

    d->testContentView = new QListView(this);
    testLayout->addWidget( d->testContentView );
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

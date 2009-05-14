//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "catalogwidget.h"
#include "tasklistview.h"
#include "pixmapbutton.h"

#include "mainwindow.h"

class CatalogWidget::Private
{
public:
    TaskListView * taskListView;
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
    layout->setSpacing(0);
    setLayout(layout);
     
    d->taskListView = new TaskListView(this);
    layout->addWidget( d->taskListView, 0,0 );

    connect(d->taskListView, SIGNAL(studyTaskActivated(QString)),
            this,            SIGNAL(studyTaskActivated(QString)));
    connect(d->taskListView, SIGNAL(currentTaskChanged(QString)),
            this,            SIGNAL(currentTaskChanged(QString)));

    //focus
    setFocusProxy( d->taskListView );
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

QString CatalogWidget::currentTaskId()
{
    return d->taskListView->currentTaskId();
}

void CatalogWidget::addNewStudy()
{
    d->taskListView->addNewStudy();
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "tasklistview.h"
#include "tasklistmodel.h"

class TaskListView::Private
{
public:
    QPointer<TaskListModel> model;
    QPointer<QMenu> contextMenu;
};

TaskListView::TaskListView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;

    setRootIsDecorated(false);
    setAutoFillBackground(true);
    setAlternatingRowColors(true);
    setFrameStyle(QFrame::NoFrame);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Create new Study") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("Remove") );

    d->model = new TaskListModel(this);
    setModel( d->model );
}

TaskListView::~TaskListView()
{
    qDebug() << "~TaskListView";
    delete d;
}

void TaskListView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( mapToGlobal(pos) );
}

void TaskListView::addNewStudy()
{
    StudyTask * task = new StudyTask( d->model );
    d->model->addTask( task );

    setCurrentIndex( d->model->indexOf(task) );
    edit( d->model->indexOf(task) );
}

void TaskListView::removeStudy()
{
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "tasklistview.h"
#include "tasklistmodel.h"
#include "tasklistfiltermodel.h"

class TaskListView::Private
{
public:
    QPointer<TaskListFilterModel> filter;
    QPointer<TaskListModel> model;
    QPointer<QMenu> contextMenu;
};

TaskListView::TaskListView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;

    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAutoFillBackground(true);
    setAlternatingRowColors(true);
    setFrameStyle(QFrame::NoFrame);
    setEditTriggers(editTriggers() ^ QAbstractItemView::DoubleClicked);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Create new Study"), this, SLOT(addNewStudy()));
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("Rename"), this, SLOT(editStudyName()), QKeySequence("F2"));
    //d->contextMenu->addSeparator();
    //d->contextMenu->addAction( tr("Properties"));

    d->model = new TaskListModel(this);
    d->filter = new TaskListFilterModel( d->model );
    d->filter->setSourceModel( d->model );
    d->filter->sort(0, Qt::AscendingOrder);
    d->filter->setFilterKeyColumn(0);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), 
            this, SLOT(activateItem(const QModelIndex &)));

    setModel( d->filter );
}

TaskListView::~TaskListView()
{
    qDebug() << "~TaskListView";
    delete d;
}

void TaskListView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = d->filter->mapToSource( currentIndex() );
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void TaskListView::addNewStudy()
{
    StudyTask * task = new StudyTask( d->model );
    task->setCategoryId( d->filter->categoryId() );
    d->model->addTask( task );

    QModelIndex index = d->filter->mapFromSource( d->model->indexOf(task) );
    setCurrentIndex( index );
    edit( index );
}

void TaskListView::editStudyName()
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;
    edit(index);
}

void TaskListView::editCurrentStudy()
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;
    activateItem(index);
}

void TaskListView::activateItem(const QModelIndex & i)
{
    if ( !i.isValid() ) return;
    QModelIndex index = d->filter->mapToSource( i );
    if ( !index.isValid() ) return;

    StudyTask * task = d->model->task( index );
    emit studyTaskActivated( task->id() );
}

void TaskListView::applyCategoryFilter(QString categoryId)
{
    if ( categoryId.isEmpty() ) {
        d->filter->clearFiltering();
        return;
    }

    d->filter->setCategoryFiltering( categoryId );
}

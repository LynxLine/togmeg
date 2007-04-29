//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "mainwindow.h"
#include "headerview.h"
#include "tasklistview.h"
#include "tasklistmodel.h"
#include "tasklistfiltermodel.h"

class TaskListView::Private
{
public:
    QPointer<TaskListFilterModel> filter;
    QPointer<TaskListModel> model;
    QPointer<QMenu> contextMenu;
    QPointer<HeaderView> header;

    QColor c1;
    QColor c2;
};

TaskListView::TaskListView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    d->header = new HeaderView(Qt::Horizontal, this);
    d->header->setFont( MainWindow::baseFont(0.95) );
    d->header->setStretchLastSection(true);

    setHeader( d->header );
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAutoFillBackground(true);

#ifdef Q_WS_MAC
    setAlternatingRowColors(false);
    {
        d->c1 = palette().color(QPalette::Base);
        d->c2 = palette().color(QPalette::AlternateBase);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, QColor(0,0,0,0));
        setPalette(palette);
    }
#else
    setAlternatingRowColors(true);
#endif

    setFrameStyle(QFrame::NoFrame);
    setItemDelegate(new TaskListItemDelegate(this));
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
    if ( model()->rowCount() )
        setCurrentIndex( model()->index(0,0) );
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
    }
    else {
        d->filter->setCategoryFiltering( categoryId );
    }
    
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) {
        if ( model()->rowCount() )
            setCurrentIndex( model()->index(0,0) );
        else
            currentTaskChanged(QString::null);
    }
}

QString TaskListView::currentTaskId()
{
    QModelIndex current = currentIndex();
    if ( !current.isValid() ) {
        return QString::null;
    }

    QModelIndex index = d->filter->mapToSource( current );

    if ( !index.isValid() ) {
        return QString::null;
    }

    StudyTask * task = d->model->task( index );
    return task->id();
}

void TaskListView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);

    if ( !current.isValid() ) {
        currentTaskChanged(QString::null);
        return;
    }

    QModelIndex index = d->filter->mapToSource( current );

    if ( !index.isValid() ) {
        currentTaskChanged(QString::null);
        return;
    }

    StudyTask * task = d->model->task( index );
    emit currentTaskChanged( task->id() );
}

void TaskListView::paintEvent(QPaintEvent * pe)
{
#ifdef Q_WS_MAC
    QRect r = pe->rect();
    QPainter p( viewport() );

    int rowHeight = 20;
    int i1 = r.y()/rowHeight-1;
    int i2 = (r.y()+r.height())/rowHeight+1;

    for (int i=i1;i<i2;i++) {
        QRect rf = QRect(r.x(), i*rowHeight, r.width(), rowHeight);
        p.fillRect(rf, i % 2 ? d->c2 : d->c1);
    }

    p.end();
#endif

    QTreeView::paintEvent(pe);
}

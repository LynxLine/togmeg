//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "taskeditorview.h"
#include "studytaskmodel.h"

class TaskEditorView::Private
{
public:
    QPointer<StudyTaskModel> model;
    QPointer<QMenu> contextMenu;
};

TaskEditorView::TaskEditorView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;

    setRootIsDecorated(false);
    setAutoFillBackground(true);
    setAlternatingRowColors(true);
    setFrameStyle(QFrame::NoFrame);
    setAllColumnsShowFocus(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    setEditTriggers(
            QAbstractItemView::DoubleClicked |
            QAbstractItemView::SelectedClicked |
            QAbstractItemView::EditKeyPressed |
            QAbstractItemView::AnyKeyPressed
        );

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Add new Question") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("&Remove"), this, SLOT(removeEntry()));

    d->model = new StudyTaskModel(this);
    setModel( d->model );

    header()->setDefaultAlignment(Qt::AlignLeft);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::Stretch);
    header()->setResizeMode(2, QHeaderView::Stretch);
}

TaskEditorView::~TaskEditorView()
{
    qDebug() << "~TaskEditorView";
    delete d;
}

void TaskEditorView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void TaskEditorView::openTask(QString taskId)
{
    d->model->load(taskId);
    setFocus();
    if ( d->model->rowCount() )
        setCurrentIndex( d->model->index(0,0) );
}

void TaskEditorView::addNewEntry()
{
    QModelIndex index = d->model->addNewEntry();
    setCurrentIndex( index );
    edit( index );
}

void TaskEditorView::removeEntry()
{
    QModelIndex index = currentIndex();
    d->model->removeEntry( index );
    scrollTo(currentIndex());
}

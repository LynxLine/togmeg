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

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Add new question-answer") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("Remove") );

    d->model = new StudyTaskModel(this);
    setModel( d->model );
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

    d->contextMenu->popup( mapToGlobal(pos) );
}

/*
void TaskEditorView::addNewStudy()
{
    StudyTask * task = new StudyTask( d->model );
    d->model->addTask( task );

    setCurrentIndex( d->model->indexOf(task) );
    edit( d->model->indexOf(task) );
}

void TaskEditorView::removeStudy()
{
}
*/
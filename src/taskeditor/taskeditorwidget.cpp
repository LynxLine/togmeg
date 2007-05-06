//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "taskeditorview.h"
#include "taskeditorwidget.h"

class TaskEditorWidget::Private
{
public:
    TaskEditorView * taskEditorView;
};

/*!
 Crestes TaskEditorWidget
 */
TaskEditorWidget::TaskEditorWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QGridLayout * layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    //task list area
    QVBoxLayout * taskLayout = new QVBoxLayout;
    taskLayout->setMargin(0);
    taskLayout->setSpacing(0);
    layout->addLayout(taskLayout, 0, 0);

    d->taskEditorView = new TaskEditorView(this);
    taskLayout->addWidget( d->taskEditorView );

    setFocusProxy( d->taskEditorView );
}

/*!
 Deletes the object.
 */
TaskEditorWidget::~TaskEditorWidget()
{
    delete d;
}

void TaskEditorWidget::addNewEntry()
{
    d->taskEditorView->addNewEntry();
}

void TaskEditorWidget::setCurrentTask(QString taskId)
{
    if (d->taskEditorView->currentTaskId() != taskId)
        d->taskEditorView->openTask(taskId);
}

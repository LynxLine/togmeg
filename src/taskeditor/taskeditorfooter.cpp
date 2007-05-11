//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "mainwindow.h"
#include "tasklistmodel.h"
#include "taskeditorfooter.h"

class TaskEditorFooter::Private {
public:
    Private() 
    :studyCount(0), currentTaskEntryCount(0)
    {;}
    QLabel * l_message;

    int studyCount;
    QString currentTaskName;
    int currentTaskEntryCount;
};

/*!
 * Creates the object.
 */
TaskEditorFooter::TaskEditorFooter(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    setFixedHeight(22);
    setFocusPolicy(Qt::NoFocus);
    setAutoFillBackground(true);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setSpacing(5);
    layout->setMargin(3);
    setLayout(layout);

    d->l_message = new QLabel;
    d->l_message->setAlignment( Qt::AlignCenter );
    d->l_message->setFont( MainWindow::baseFont(0.95) );
    layout->addWidget( d->l_message );

    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, "#505050");
    palette.setBrush(QPalette::Window, QPixmap(":/images/icons/bg-bottom-category.png"));
    setPalette(palette);
}

/*!
 * Deletes it.
 */
TaskEditorFooter::~TaskEditorFooter()
{
    delete d;
}

void TaskEditorFooter::setStudyCount(int count)
{
    d->studyCount = count;
    updateMessageLabel();
}

void TaskEditorFooter::setCurrentTask(QString taskId)
{
    d->currentTaskName.clear();
    d->currentTaskEntryCount = 0;

    if ( !taskId.isEmpty() ) {
        StudyTask * task = TaskListModel::instance()->task( taskId );
        if (task) {
            d->currentTaskName = task->name();
            d->currentTaskEntryCount = task->entryCount();
        }
    }

    updateMessageLabel();
}

void TaskEditorFooter::updateMessageLabel()
{
    if ( d->currentTaskName.isEmpty() ) {
        d->l_message->setText(
            tr("Studies: %1")
                .arg( d->studyCount )
        );
    }
    else {
        d->l_message->setText(
            tr("Studies: %1, Current: %2, %3 Questions")
                .arg( d->studyCount )
                .arg( d->currentTaskName )
                .arg( d->currentTaskEntryCount )
        );
    }
}

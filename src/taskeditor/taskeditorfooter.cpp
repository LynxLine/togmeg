//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "mainwindow.h"
#include "pixmapbutton.h"
#include "tasklistmodel.h"
#include "taskeditorfooter.h"

class TaskEditorFooter::Private {
public:
    Private() 
    :studyCount(0), currentTaskEntryCount(0)
    {;}
    QLabel * l_message;
    PixmapButton * b_qaqaMode;
    PixmapButton * b_qqaaMode;
    TaskEditorView::NextItemMode nextItemMode;

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
    d->nextItemMode = TaskEditorView::NextItemMode(-1);//undefined

    setFixedHeight(22);
    setFocusPolicy(Qt::NoFocus);
    setAutoFillBackground(true);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);

    QFrame * vline2 = new QFrame;
    {
        QPalette palette = vline2->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        vline2->setPalette(palette);
        vline2->setFrameStyle(QFrame::VLine | QFrame::Plain);
        vline2->setFixedWidth(1);
        vline2->setLineWidth(1);
    }

    QFrame * vline3 = new QFrame;
    {
        QPalette palette = vline3->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        vline3->setPalette(palette);
        vline3->setFrameStyle(QFrame::VLine | QFrame::Plain);
        vline3->setFixedWidth(1);
        vline3->setLineWidth(1);
    }

    d->b_qqaaMode = new PixmapButton(
        ":/images/icons/qqaa-order-button-on.png",
        ":/images/icons/qqaa-order-button-on.png",
        ":/images/icons/qqaa-order-button-down.png"
    );
    layout->addWidget( d->b_qqaaMode );
    layout->addWidget( vline3 );

    d->b_qaqaMode = new PixmapButton(
        ":/images/icons/qaqa-order-button.png",
        ":/images/icons/qaqa-order-button.png",
        ":/images/icons/qaqa-order-button-down.png"
    );
    layout->addWidget( d->b_qaqaMode );
    layout->addWidget( vline2 );


    connect(d->b_qaqaMode, SIGNAL(pressed()), this, SLOT(toQAQAMode()));
    connect(d->b_qqaaMode, SIGNAL(pressed()), this, SLOT(toQQAAMode()));

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

void TaskEditorFooter::toQAQAMode()
{
    setNextItemMode(TaskEditorView::QAQAMode);
}

void TaskEditorFooter::toQQAAMode()
{
    setNextItemMode(TaskEditorView::QQAAMode);
}

void TaskEditorFooter::setNextItemMode(TaskEditorView::NextItemMode m)
{
    if ( d->nextItemMode == m ) return;

    if ( m == TaskEditorView::QAQAMode ) {
        d->b_qaqaMode->setPixmap  ( QPixmap(":/images/icons/qaqa-order-button-on.png") );
        d->b_qaqaMode->setPixmapHl( QPixmap(":/images/icons/qaqa-order-button-on.png") );
        d->b_qqaaMode->setPixmap  ( QPixmap(":/images/icons/qqaa-order-button.png") );
        d->b_qqaaMode->setPixmapHl( QPixmap(":/images/icons/qqaa-order-button.png") );
    }
    else if ( m == TaskEditorView::QQAAMode ) {
        d->b_qaqaMode->setPixmap  ( QPixmap(":/images/icons/qaqa-order-button.png") );
        d->b_qaqaMode->setPixmapHl( QPixmap(":/images/icons/qaqa-order-button.png") );
        d->b_qqaaMode->setPixmap  ( QPixmap(":/images/icons/qqaa-order-button-on.png") );
        d->b_qqaaMode->setPixmapHl( QPixmap(":/images/icons/qqaa-order-button-on.png") );
    }

    d->b_qaqaMode->update();
    d->b_qqaaMode->update();

    d->nextItemMode = m;
    emit nextItemModeChanged(m);
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

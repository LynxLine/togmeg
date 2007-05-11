//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "tabbar.h"
#include "mainwindow.h"
#include "pixmapbutton.h"
#include "gradientwidget.h"
#include "taskeditorview.h"
#include "taskeditorfooter.h"
#include "taskeditorwidget.h"
#include "playpropertieswidget.h"
#include "studypropertieswidget.h"
#include "exampropertieswidget.h"

class TaskEditorWidget::Private
{
public:
    TaskEditorView * taskEditorView;
    TaskEditorFooter * taskEditorFooter;

    //GradientWidget * generalPanel;
    ExamPropertiesWidget * examinePanel;
    StudyPropertiesWidget * studyPanel;
    PlayPropertiesWidget * playPanel;
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
    layout->addLayout(taskLayout, 0,0 );

    d->taskEditorView = new TaskEditorView(this);
    taskLayout->addWidget( d->taskEditorView );

    QFrame * hline1 = new QFrame;
    {
        QPalette palette = hline1->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        hline1->setPalette(palette);
        hline1->setFrameStyle(QFrame::HLine | QFrame::Plain);
        hline1->setFixedHeight(1);
        hline1->setLineWidth(1);
    }
    layout->addWidget( hline1, 1,0 );

    //footer
    d->taskEditorFooter = new TaskEditorFooter;
    layout->addWidget( d->taskEditorFooter, 2,0 );

    d->taskEditorFooter->setNextItemMode( d->taskEditorView->nextItemMode() );
    connect(d->taskEditorFooter, SIGNAL(nextItemModeChanged(TaskEditorView::NextItemMode)),
            d->taskEditorView, SLOT(setNextItemMode(TaskEditorView::NextItemMode)));

    QFrame * vline = new QFrame;
    {
        QPalette palette = vline->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        vline->setPalette(palette);
        vline->setFrameStyle(QFrame::VLine | QFrame::Plain);
        vline->setFixedWidth(1);
        vline->setLineWidth(1);
    }
    layout->addWidget( vline, 0,1, 3,1);

    //properties layout
    QVBoxLayout * propertiesAreaLayout = new QVBoxLayout;
    propertiesAreaLayout->setMargin(0);
    propertiesAreaLayout->setSpacing(0);
    layout->addLayout( propertiesAreaLayout, 0,2, 3,1 );

    TabBar * tab = new TabBar;
    tab->setFixedWidth(250);
    tab->setFont(MainWindow::baseFont(0.90, QFont::Bold));
    propertiesAreaLayout->addWidget( tab );

    QStackedWidget * stack = new QStackedWidget;
    stack->setFixedWidth(250);
    propertiesAreaLayout->addWidget( stack );

    connect(tab, SIGNAL(currentChanged(int)),
            stack, SLOT(setCurrentIndex(int)));

    d->examinePanel = new ExamPropertiesWidget;
    d->studyPanel   = new StudyPropertiesWidget;
    d->playPanel    = new PlayPropertiesWidget;

    stack->insertWidget( tab->addTab( tr("Play")),    d->playPanel    );
    stack->insertWidget( tab->addTab( tr("Study")),   d->studyPanel   );
    stack->insertWidget( tab->addTab( tr("Exam")),    d->examinePanel );

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

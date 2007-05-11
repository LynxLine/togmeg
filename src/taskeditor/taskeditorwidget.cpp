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

class TaskEditorWidget::Private
{
public:
    TaskEditorView * taskEditorView;
    TaskEditorFooter * taskEditorFooter;

    //GradientWidget * generalPanel;
    GradientWidget * examinePanel;
    GradientWidget * studyPanel;
    GradientWidget * playPanel;
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

    d->taskEditorFooter = new TaskEditorFooter;
    layout->addWidget( d->taskEditorFooter, 2,0 );

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

    /*
    QTabBar * tab = new QTabBar;
    tab->setAutoFillBackground(true);
    tab->setFixedWidth(250);
    {
        QPalette palette = tab->palette();
        palette.setColor(QPalette::Window, "#A0A0A0");
        space->setPalette(palette);
        tab->setPalette(palette);
    }
    propertiesAreaLayout->addWidget( tab );
    */

    TabBar * tab = new TabBar;
    tab->setFixedWidth(250);
    tab->setFont(MainWindow::baseFont(0.90, QFont::Bold));
    propertiesAreaLayout->addWidget( tab );

    QStackedWidget * stack = new QStackedWidget;
    stack->setFixedWidth(250);
    propertiesAreaLayout->addWidget( stack );

    connect(tab, SIGNAL(currentChanged(int)),
            stack, SLOT(setCurrentIndex(int)));

    d->examinePanel = new GradientWidget;
    d->examinePanel->gradient().setColorAt(0, "#E8E8E8");
    d->examinePanel->gradient().setColorAt(1, "#D0D0D0");

    d->studyPanel   = new GradientWidget;
    d->studyPanel->gradient().setColorAt(0, "#E8E8E8");
    d->studyPanel->gradient().setColorAt(1, "#D0D0D0");

    d->playPanel    = new GradientWidget;
    d->playPanel->gradient().setColorAt(0, "#E8E8E8");
    d->playPanel->gradient().setColorAt(1, "#D0D0D0");

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

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "TogMegWindow.h"
#include "pixmapbutton.h"
#include "gradientwidget.h"
#include "CramFileView.h"
#include "Footer.h"
#include "CramFileWidget.h"

class TaskEditorWidget::Private
{
public:
    TaskEditorView * taskEditorView;
    TaskEditorFooter * taskEditorFooter;
};

/*!
 Crestes TaskEditorWidget
 */
TaskEditorWidget::TaskEditorWidget(CramFileModel * model, QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QGridLayout * layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    d->taskEditorView = new TaskEditorView(model, this);
    layout->addWidget( d->taskEditorView );

    QFrame * hline1 = new QFrame; {
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

    setFocusProxy( d->taskEditorView );
}

TaskEditorView * TaskEditorWidget::view() const
{
    return d->taskEditorView;
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

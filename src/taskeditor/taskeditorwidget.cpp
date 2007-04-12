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
    layout->setSpacing(1);
    setLayout(layout);

    //task list area
    QVBoxLayout * taskLayout = new QVBoxLayout;
    taskLayout->setMargin(0);
    taskLayout->setSpacing(5);
    layout->addLayout(taskLayout, 0, 0);

    d->taskEditorView = new TaskEditorView(this);
    taskLayout->addWidget( d->taskEditorView );

    //bottom footer
    QHBoxLayout * footerAreaLayout = new QHBoxLayout;
    footerAreaLayout->setMargin(0);
    footerAreaLayout->setSpacing(5);
    layout->addLayout( footerAreaLayout, 1, 0 );

    QToolButton * addCategory = new QToolButton;
    QToolButton * remCategory = new QToolButton;

    addCategory->setText("+Q-A");
    remCategory->setText("-Q-A");

    /*
    connect(addCategory, SIGNAL(clicked()), d->catalogView, SLOT(addSubCategory()));
    connect(remCategory, SIGNAL(clicked()), d->catalogView, SLOT(removeCategory()));
    */

    footerAreaLayout->addWidget( addCategory );
    footerAreaLayout->addWidget( remCategory );
    footerAreaLayout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum) );
}

/*!
 Deletes the object.
 */
TaskEditorWidget::~TaskEditorWidget()
{
    delete d;
}

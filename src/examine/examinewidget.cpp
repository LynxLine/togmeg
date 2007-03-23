//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "svgwidget.h"
#include "examinewidget.h"
#include "mainwindow.h"

class ExamineWidget::Private
{
public:
    SvgWidget * svgWidget;
};

/*!
 Crestes ExamineWidget
 */
ExamineWidget::ExamineWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setMargin(10);
    layout->setSpacing(5);
    setLayout(layout);

    QHBoxLayout * manageTaskLayout = new QHBoxLayout;
    manageTaskLayout->setMargin(0);
    manageTaskLayout->setSpacing(5);
    layout->addLayout( manageTaskLayout );

    QPushButton * pauseTask = new QPushButton("Pause");
    QPushButton * stopTask = new QPushButton("Stop");

    connect(pauseTask, SIGNAL(clicked()), _action("app/stop"), SLOT(trigger()));
    connect(stopTask,  SIGNAL(clicked()), _action("app/stop"), SLOT(trigger()));

    manageTaskLayout->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Minimum) );
    manageTaskLayout->addWidget( pauseTask );
    manageTaskLayout->addWidget( stopTask );

    d->svgWidget = new SvgWidget(this);
    layout->addWidget( d->svgWidget );

    layout->addItem(new QSpacerItem(10, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));
    layout->addWidget(new QLineEdit);
    layout->addItem(new QSpacerItem(10, 30, QSizePolicy::Minimum, QSizePolicy::Fixed));

    QProgressBar * pb_time = new QProgressBar;
    pb_time->setRange(0,0);
    pb_time->setValue(0);
    pb_time->setTextVisible(false);
    layout->addWidget(pb_time);
}

/*!
 Deletes the object.
 */
ExamineWidget::~ExamineWidget()
{
    delete d;
}

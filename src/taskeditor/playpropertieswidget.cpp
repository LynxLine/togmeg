//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "mainwindow.h"
#include "timelinewidget.h"
#include "playpropertieswidget.h"

class PlayPropertiesWidget::Private {
public:
    QPointer<TimeLineWidget> timeLineWidget;
};

/*!
 * Creates the object.
 */
PlayPropertiesWidget::PlayPropertiesWidget(QWidget * parent)
:GradientWidget(parent)
{
    d = new Private;
    gradient().setColorAt(0, "#E8E8E8");
    gradient().setColorAt(1, "#D0D0D0");
    setContentsMargins(15, 10, 15, 20);
    setFont( MainWindow::baseFont(0.95) );

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setMargin(0);
    setLayout(layout);

    QGridLayout * grid = new QGridLayout;
    grid->setSpacing(5);
    grid->setMargin(0);
    layout->addLayout(grid);

    grid->addWidget( new QCheckBox(tr("Randomize questions")),        0,0, 1,4 );
    grid->addItem  ( new QSpacerItem(10,5, QSizePolicy::Minimum, QSizePolicy::Fixed), 1,0);
    grid->addWidget( new QRadioButton(tr("Loop until Stop pressed")), 2,0, 1,4 );
    grid->addItem  ( new QSpacerItem(10,5, QSizePolicy::Minimum, QSizePolicy::Fixed), 3,0);
    grid->addWidget( new QRadioButton(tr("Repeat all entries")),      4,0, 1,4 );

    grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 5,0 );
    grid->addWidget( new QSpinBox(), 5,1 );
    grid->addWidget( new QLabel(tr("times")), 5,2 );
    grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 5,3 );

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    d->timeLineWidget = new TimeLineWidget;
    layout->addWidget( d->timeLineWidget );

}

/*!
 * Deletes it.
 */
PlayPropertiesWidget::~PlayPropertiesWidget()
{
    delete d;
}

void PlayPropertiesWidget::setCurrentTask(QString taskId)
{
}

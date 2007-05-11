//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
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

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setMargin(0);
    setLayout(layout);

    QGridLayout * grid = new QGridLayout;
    grid->setSpacing(5);
    grid->setMargin(0);
    layout->addLayout(grid);

    grid->addWidget( new QRadioButton(tr("Loop")), 0,0, 1,3 );
    grid->addWidget( new QRadioButton(tr("Repeat")), 1,0 );
    grid->addWidget( new QSpinBox(), 1,1 );
    grid->addWidget( new QLabel(tr("times")), 1,2 );

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

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "timelinewidget.h"
#include "exampropertieswidget.h"

class ExamPropertiesWidget::Private {
public:
    QPointer<TimeLineWidget> timeLineWidget;
};

/*!
 * Creates the object.
 */
ExamPropertiesWidget::ExamPropertiesWidget(QWidget * parent)
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

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    d->timeLineWidget = new TimeLineWidget;
    layout->addWidget( d->timeLineWidget );

}

/*!
 * Deletes it.
 */
ExamPropertiesWidget::~ExamPropertiesWidget()
{
    delete d;
}

void ExamPropertiesWidget::setCurrentTask(QString taskId)
{
}

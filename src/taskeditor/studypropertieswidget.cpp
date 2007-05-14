//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "mainwindow.h"
#include "timelinewidget.h"
#include "studypropertieswidget.h"

class StudyPropertiesWidget::Private {
public:
    QPointer<TimeLineWidget> timeLineWidget;
};

/*!
 * Creates the object.
 */
StudyPropertiesWidget::StudyPropertiesWidget(QWidget * parent)
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

    grid->addWidget( new QCheckBox(tr("Split long study by parts of")), 0,0, 1,4 );

    grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 1,0 );
    grid->addWidget( new QSpinBox(), 1,1 );
    grid->addWidget( new QLabel(tr("questions")), 1,2 );
    grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 1,3 );

    grid->addItem(new QSpacerItem(10,15, QSizePolicy::Minimum, QSizePolicy::Fixed), 2,0);

    grid->addWidget( new QLabel(tr("Mark question as studied after")), 3,0, 1,4 );

    grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 4,0 );
    grid->addWidget( new QSpinBox(), 4,1 );
    grid->addWidget( new QLabel(tr("correct answers")), 4,2 );
    grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 4,3 );

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    d->timeLineWidget = new TimeLineWidget;
    layout->addWidget( d->timeLineWidget );
}

/*!
 * Deletes it.
 */
StudyPropertiesWidget::~StudyPropertiesWidget()
{
    delete d;
}

void StudyPropertiesWidget::setCurrentTask(QString taskId)
{
}

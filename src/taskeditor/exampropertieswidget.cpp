//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "mainwindow.h"
#include "tasklistmodel.h"
#include "exampropertieswidget.h"

class ExamPropertiesWidget::Private {
public:
    StudyTask * currentTask;
    QCheckBox * cb_randomize;
};

/*!
 * Creates the object.
 */
ExamPropertiesWidget::ExamPropertiesWidget(QWidget * parent)
:GradientWidget(parent)
{
    d = new Private;
    d->currentTask = 0L;

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

    d->cb_randomize = new QCheckBox(tr("Randomize questions"));
    connect(d->cb_randomize, SIGNAL(stateChanged(int)), 
            this, SLOT(randomizeStateChanged(int)));

    grid->addWidget( d->cb_randomize, 0,0, 1,4 );
    grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Fixed), 1,0);

    grid->addWidget( new QCheckBox(tr("Process only")),      2,0, 1,4 );

    grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 3,0 );
    grid->addWidget( new QSpinBox(), 3,1 );
    grid->addWidget( new QLabel(tr("questions of study")), 3,2 );
    grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 3,3 );

    grid->addItem  ( new QSpacerItem(10,15, QSizePolicy::Minimum, QSizePolicy::Fixed), 4,0);

    grid->addWidget( new QCheckBox(tr("Limit exam time to")),      5,0, 1,4 );

    grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 6,0 );
    grid->addWidget( new QSpinBox(), 6,1 );
    grid->addWidget( new QLabel(tr("minutes")), 6,2 );
    grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 6,3 );


    layout->addItem(new QSpacerItem(10,20, QSizePolicy::Minimum, QSizePolicy::Fixed));

    {
        QHBoxLayout * hbox = new QHBoxLayout;
        hbox->setSpacing(2);
        hbox->setMargin(0);

        QLabel * l = new QLabel(tr("<b>Timeline</b>"));
        hbox->addWidget(l);

        {
            QFrame * hline = new QFrame;
            hline->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
            hline->setFrameStyle(QFrame::HLine | QFrame::Sunken);
            hline->setMidLineWidth(0);
            hline->setLineWidth(1);
            hbox->addWidget( hline );
        }

        layout->addLayout( hbox );
        layout->addItem(new QSpacerItem(10,100, QSizePolicy::Minimum, QSizePolicy::Fixed));
    }

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));
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
    StudyTask * t = TaskListModel::instance()->task( taskId );
    d->currentTask = t;

    if ( !t ) return;

    d->cb_randomize->setChecked( t->property("exam_randomize").toBool() );
}

void ExamPropertiesWidget::randomizeStateChanged(int state)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_randomize", state == Qt::Checked);
}

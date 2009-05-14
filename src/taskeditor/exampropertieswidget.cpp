//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "crammero.h"
#include "studytask.h"
#include "mainwindow.h"
#include "exampropertieswidget.h"

class ExamPropertiesWidget::Private {
public:
    StudyTask * currentTask;
    QCheckBox * cb_randomize;

    QCheckBox * cb_processOnly;
    QSpinBox * sb_processOnly;
    QLabel * l_processOnly;

    QRadioButton * rb_limitExamTime;
    QSpinBox * sb_limitExamTime;
    QComboBox * cb_limitExamTime;

    QRadioButton * rb_timeForQuestion;
    QSpinBox * sb_timeForQuestion;
    QComboBox * cb_timeForQuestion;

    QRadioButton * rb_basedOnTyping;
    QSpinBox * sb_basedOnTyping;
    QComboBox * cb_basedOnTyping;
    QLabel * l_basedOnTyping;
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

    {
        QGridLayout * grid = new QGridLayout;
        grid->setSpacing(5);
        grid->setMargin(0);
        layout->addLayout(grid);

        d->cb_randomize = new QCheckBox(tr("Randomize questions"));
        d->cb_randomize->setFocusPolicy(Qt::NoFocus);
        connect(d->cb_randomize, SIGNAL(stateChanged(int)), 
                this, SLOT(randomizeStateChanged(int)));

        grid->addWidget( d->cb_randomize, 0,0, 1,4 );
        grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Fixed), 1,0);

        d->cb_processOnly = new QCheckBox(tr("Process only"));
        d->cb_processOnly->setFocusPolicy(Qt::NoFocus);
        connect(d->cb_processOnly, SIGNAL(stateChanged(int)), 
                this, SLOT(processOnlyStateChanged(int)));

        d->sb_processOnly = new QSpinBox;
        d->sb_processOnly->setAlignment(Qt::AlignRight);
        d->sb_processOnly->setFocusPolicy(Qt::NoFocus);
        d->sb_processOnly->setMaximum(9999);
        d->sb_processOnly->setMinimum(1);
        connect(d->sb_processOnly, SIGNAL(valueChanged(int)), 
                this, SLOT(processOnlyCountChanged(int)));

        d->l_processOnly = new QLabel(tr(" questions"));

        grid->addWidget( d->cb_processOnly, 2,0, 1,4 );
        grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 3,0 );
        grid->addWidget( d->sb_processOnly, 3,1 );
        grid->addWidget( d->l_processOnly, 3,2 );
        grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 3,3 );
    }

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
    }

    {
        QGridLayout * grid = new QGridLayout;
        grid->setSpacing(5);
        grid->setMargin(0);
        layout->addLayout(grid);

        //
        d->rb_limitExamTime = new QRadioButton(tr("Limit exam time to"));
        d->rb_limitExamTime->setFocusPolicy(Qt::NoFocus);
        connect(d->rb_limitExamTime, SIGNAL(toggled(bool)), 
                this, SLOT(limitExamTimeToggled(bool)));

        d->sb_limitExamTime = new QSpinBox;
        d->sb_limitExamTime->setAlignment(Qt::AlignRight);
        d->sb_limitExamTime->setFocusPolicy(Qt::NoFocus);
        d->sb_limitExamTime->setMaximum(9999);
        d->sb_limitExamTime->setMinimum(1);
        connect(d->sb_limitExamTime, SIGNAL(valueChanged(int)), 
                this, SLOT(limitExamTimeChanged(int)));

        d->cb_limitExamTime = new QComboBox;
        d->cb_limitExamTime->setFocusPolicy(Qt::NoFocus);
        d->cb_limitExamTime->insertItem(CB_TIME_SECONDS, tr("seconds"));
        d->cb_limitExamTime->insertItem(CB_TIME_MINUTES, tr("minutes"));
        connect(d->cb_limitExamTime, SIGNAL(currentIndexChanged(int)), 
                this, SLOT(limitExamTimeUnitChanged(int)));

        //
        d->rb_timeForQuestion = new QRadioButton(tr("Fixed time for each question"));
        d->rb_timeForQuestion->setFocusPolicy(Qt::NoFocus);
        connect(d->rb_timeForQuestion, SIGNAL(toggled(bool)), 
                this, SLOT(timeForQuestionToggled(bool)));

        d->sb_timeForQuestion = new QSpinBox;
        d->sb_timeForQuestion->setAlignment(Qt::AlignRight);
        d->sb_timeForQuestion->setFocusPolicy(Qt::NoFocus);
        d->sb_timeForQuestion->setMaximum(9999);
        d->sb_timeForQuestion->setMinimum(1);
        connect(d->sb_timeForQuestion, SIGNAL(valueChanged(int)), 
                this, SLOT(timeForQuestionChanged(int)));

        d->cb_timeForQuestion = new QComboBox;
        d->cb_timeForQuestion->setFocusPolicy(Qt::NoFocus);
        d->cb_timeForQuestion->insertItem(CB_TIME_SECONDS, tr("seconds"));
        d->cb_timeForQuestion->insertItem(CB_TIME_MINUTES, tr("minutes"));
        connect(d->cb_timeForQuestion, SIGNAL(currentIndexChanged(int)), 
                this, SLOT(timeForQuestionUnitChanged(int)));

        //
        d->rb_basedOnTyping = new QRadioButton(tr("Based on typing speed with"));
        d->rb_basedOnTyping->setFocusPolicy(Qt::NoFocus);
        connect(d->rb_basedOnTyping, SIGNAL(toggled(bool)), 
                this, SLOT(basedOnTypingToggled(bool)));

        d->sb_basedOnTyping = new QSpinBox;
        d->sb_basedOnTyping->setAlignment(Qt::AlignRight);
        d->sb_basedOnTyping->setFocusPolicy(Qt::NoFocus);
        d->sb_basedOnTyping->setMaximum(9999);
        d->sb_basedOnTyping->setMinimum(1);
        connect(d->sb_basedOnTyping, SIGNAL(valueChanged(int)), 
                this, SLOT(basedOnTypingChanged(int)));

        d->cb_basedOnTyping = new QComboBox;
        d->cb_basedOnTyping->setFocusPolicy(Qt::NoFocus);
        d->cb_basedOnTyping->insertItem(CB_TIME_SECONDS, tr("seconds"));
        d->cb_basedOnTyping->insertItem(CB_TIME_MINUTES, tr("minutes"));
        connect(d->cb_basedOnTyping, SIGNAL(currentIndexChanged(int)), 
                this, SLOT(basedOnTypingUnitChanged(int)));

        d->l_basedOnTyping = new QLabel(tr("7 X 0.50 +5 = 3.5 secs"));
        {
            QPalette palette = d->l_basedOnTyping->palette();
            palette.setColor(QPalette::WindowText, "#5050FF");
            d->l_basedOnTyping->setPalette(palette);
            d->l_basedOnTyping->setWordWrap(true);
        }

        //
        grid->addItem  ( new QSpacerItem(10,15, QSizePolicy::Minimum, QSizePolicy::Fixed), 1,0);
        grid->addWidget( d->rb_basedOnTyping, 2,0, 1,4 );

        grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 3,0 );
        grid->addWidget( d->sb_basedOnTyping, 3,1 );
        grid->addWidget( d->cb_basedOnTyping, 3,2 );
        grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 3,3 );

        grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 4,0 );
        grid->addWidget( d->l_basedOnTyping, 4,1, 1,3 );

        //
        grid->addItem  ( new QSpacerItem(10,15, QSizePolicy::Minimum, QSizePolicy::Fixed), 5,0);
        grid->addWidget( d->rb_timeForQuestion, 6,0, 1,4 );

        grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 7,0 );
        grid->addWidget( d->sb_timeForQuestion, 7,1 );
        grid->addWidget( d->cb_timeForQuestion, 7,2 );
        grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 7,3 );

        //
        grid->addItem  ( new QSpacerItem(10,15, QSizePolicy::Minimum, QSizePolicy::Fixed), 8,0);
        grid->addWidget( d->rb_limitExamTime, 9,0, 1,4 );

        grid->addItem  ( new QSpacerItem(15,10, QSizePolicy::Fixed, QSizePolicy::Fixed), 10,0 );
        grid->addWidget( d->sb_limitExamTime, 10,1 );
        grid->addWidget( d->cb_limitExamTime, 10,2 );
        grid->addItem  ( new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed), 10,3 );
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
    /*
    StudyTask * t = TaskListModel::instance()->task( taskId );
    d->currentTask = t;

    if ( !t ) return;

    int processOnlyCount = t->property("exam_processOnlyCount").toInt();

    d->cb_randomize->setChecked( t->property("exam_randomize").toBool() );
    d->cb_processOnly->setChecked( t->property("exam_processOnly").toBool() );
    d->l_processOnly->setEnabled( t->property("exam_processOnly").toBool() );
    d->sb_processOnly->setEnabled( t->property("exam_processOnly").toBool() );
    d->sb_processOnly->setValue( processOnlyCount );

    //load first
    int basedOnTyping = t->property("exam_basedOnTypingValue").toInt();
    int limitExamTime = t->property("exam_limitExamTimeValue").toInt();
    int timeForQuestion = t->property("exam_timeForQuestionValue").toInt();

    int basedOnTypingUnit = t->property("exam_basedOnTypingUnit").toInt();
    int limitExamTimeUnit = t->property("exam_limitExamTimeUnit").toInt();
    int timeForQuestionUnit = t->property("exam_timeForQuestionUnit").toInt();

    d->rb_basedOnTyping->setChecked( t->property("exam_basedOnTyping").toBool() );
    d->rb_limitExamTime->setChecked( t->property("exam_limitExamTime").toBool() );
    d->rb_timeForQuestion->setChecked( t->property("exam_timeForQuestion").toBool() );

    if ( !d->rb_limitExamTime->isChecked() && 
         !d->rb_timeForQuestion->isChecked() &&
         !d->rb_basedOnTyping->isChecked()) {
        d->rb_basedOnTyping->setChecked(true);
    }

    basedOnTypingToggled( d->rb_basedOnTyping->isChecked() );
    limitExamTimeToggled( d->rb_limitExamTime->isChecked() );
    timeForQuestionToggled( d->rb_timeForQuestion->isChecked() );

    if ( basedOnTyping <=0 ) basedOnTyping = 5;
    if ( timeForQuestion <=0 ) timeForQuestion = 10;
    if ( limitExamTime <=0 ) limitExamTime = 10 * d->currentTask->entryCount();

    d->sb_basedOnTyping->setValue( basedOnTyping );
    d->sb_limitExamTime->setValue( limitExamTime );
    d->sb_timeForQuestion->setValue( timeForQuestion );

    d->cb_basedOnTyping->setCurrentIndex( basedOnTypingUnit );
    d->cb_limitExamTime->setCurrentIndex( limitExamTimeUnit );
    d->cb_timeForQuestion->setCurrentIndex( timeForQuestionUnit );
     */
}

void ExamPropertiesWidget::randomizeStateChanged(int state)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_randomize", state == Qt::Checked);
}

void ExamPropertiesWidget::processOnlyStateChanged(int state)
{
    d->sb_processOnly->setEnabled( state == Qt::Checked );
    d->l_processOnly->setEnabled( state == Qt::Checked );

    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_processOnly", state == Qt::Checked);
    if ( state == Qt::Checked )
        d->sb_processOnly->setValue( d->currentTask->entryCount() );
}

void ExamPropertiesWidget::processOnlyCountChanged(int count)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_processOnlyCount", count);
}

//
void ExamPropertiesWidget::limitExamTimeToggled(bool f)
{
    d->sb_limitExamTime->setEnabled( f );
    d->cb_limitExamTime->setEnabled( f );

    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_limitExamTime", f);
    if ( f ) {
        d->cb_limitExamTime->setCurrentIndex( CB_TIME_SECONDS );
        d->sb_limitExamTime->setValue( 5 * d->currentTask->entryCount() );
    }
}

void ExamPropertiesWidget::limitExamTimeChanged(int value)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_limitExamTimeValue", value);
}

void ExamPropertiesWidget::limitExamTimeUnitChanged(int timeUnit)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_limitExamTimeUnit", timeUnit);
}

//
void ExamPropertiesWidget::timeForQuestionToggled(bool f)
{
    d->sb_timeForQuestion->setEnabled( f );
    d->cb_timeForQuestion->setEnabled( f );

    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_timeForQuestion", f);
    if ( f ) {
        d->cb_timeForQuestion->setCurrentIndex( CB_TIME_SECONDS );
        d->sb_timeForQuestion->setValue( 5 );
    }
}

void ExamPropertiesWidget::timeForQuestionChanged(int value)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_timeForQuestionValue", value);
}

void ExamPropertiesWidget::timeForQuestionUnitChanged(int timeUnit)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_timeForQuestionUnit", timeUnit);
}

//
void ExamPropertiesWidget::basedOnTypingToggled(bool f)
{
    d->sb_basedOnTyping->setEnabled( f );
    d->cb_basedOnTyping->setEnabled( f );

    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_basedOnTyping", f);
    if ( f ) {
        d->l_basedOnTyping->setText(tr("typing speed = %1").arg( app::typingSpeed() ));
        d->cb_basedOnTyping->setCurrentIndex( CB_TIME_SECONDS );
        d->sb_basedOnTyping->setValue( 2 );
    }
}

void ExamPropertiesWidget::basedOnTypingChanged(int value)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_basedOnTypingValue", value);
}

void ExamPropertiesWidget::basedOnTypingUnitChanged(int timeUnit)
{
    if ( !d->currentTask ) return;
    d->currentTask->setProperty("exam_basedOnTypingUnit", timeUnit);
}

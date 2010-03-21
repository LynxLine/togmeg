//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "pixmapbutton.h"
#include "examinewidget.h"
#include "questionwidget.h"
#include "answerwidget.h"
#include "examinator.h"
#include "mainwindow.h"

class ExamineWidget::Private
{
public:
    QuestionWidget * questionWidget;
    AnswerWidget * answerWidget;
    QProgressBar * progress;
    Examinator * examinator;
    QLabel * l_processing;
    PixmapButton * b_play;

    QString taskName;
};

/*!
 Crestes ExamineWidget
 */
ExamineWidget::ExamineWidget(Examinator * examinator, QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    d->examinator = examinator;

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    QVBoxLayout * contentLayout = new QVBoxLayout;
    contentLayout->setMargin(20);
    contentLayout->setSpacing(5);
    
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout( contentLayout );
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    //question

    QHBoxLayout * questionLayout = new QHBoxLayout;
    questionLayout->setSpacing(0);
    questionLayout->setMargin(0);
    contentLayout->addLayout( questionLayout );

    d->questionWidget = new QuestionWidget;
    questionLayout->addWidget(d->questionWidget, 75);

    //answer

    QHBoxLayout * answerLayout = new QHBoxLayout;
    answerLayout->setSpacing(0);
    answerLayout->setMargin(0);
    contentLayout->addLayout( answerLayout );

    d->answerWidget = new AnswerWidget;
    answerLayout->addWidget(d->answerWidget, 50);

    setFocusProxy( d->answerWidget );

    QFrame * hline = new QFrame;
    {
        QPalette palette = hline->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        hline->setPalette(palette);
        hline->setFrameStyle(QFrame::HLine | QFrame::Plain);
        hline->setFixedHeight(1);
        hline->setLineWidth(1);
    }
    layout->addWidget( hline );

    QWidget * footer = new QWidget;
    {
        footer->setFocusPolicy(Qt::NoFocus);
        QPalette palette = footer->palette();
        palette.setColor(QPalette::Window, "#D9D9D9");
        footer->setAutoFillBackground(true);
        footer->setFixedHeight( 60 );
        footer->setPalette(palette);
    }
    layout->addWidget(footer);

    QGridLayout * footerLayout = new QGridLayout;
    footerLayout->setSpacing(10);
    footerLayout->setMargin(0);
    footer->setLayout(footerLayout);

    d->b_play = new PixmapButton;
    d->l_processing = new QLabel(tr("Processing...\n"));

    d->progress = new QProgressBar;
    d->progress->setRange(0,100);
    d->progress->setValue(0);

    footerLayout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum),0,0);
    footerLayout->addWidget(d->b_play, 0,1);
    footerLayout->addWidget(d->l_processing, 0,2);
    footerLayout->addItem(new QSpacerItem(20,10, QSizePolicy::Fixed, QSizePolicy::Minimum),0,3);
    footerLayout->addWidget(d->progress,  0,4);
    footerLayout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum),0,5);

    footerLayout->setColumnStretch(0,05);
    footerLayout->setColumnStretch(2,20);
    footerLayout->setColumnStretch(4,70);
    footerLayout->setColumnStretch(5,05);

    connect(d->examinator, SIGNAL(tick(int)),
            d->progress,     SLOT(setValue(int)));
    connect(d->examinator, SIGNAL(taskNameChanged(QString)),
            this,            SLOT(setTaskName(QString)));

    connect(d->examinator, SIGNAL(modeChanged(Examinator::Mode)),
            this,            SLOT(setExaminatorMode(Examinator::Mode)));
    connect(d->examinator, SIGNAL(stateChanged(Examinator::State)),
            this,            SLOT(setExaminatorState(Examinator::State)));

    connect(d->examinator,   SIGNAL(currentQuestionChanged(QString)),
            d->questionWidget, SLOT(setQuestion(QString)));
    connect(d->examinator,   SIGNAL(currentAnswerChanged(QString)),
            d->answerWidget,   SLOT(setAnswer(QString)));

    connect(d->answerWidget, SIGNAL(userAnswerChanged(QString)),
            d->examinator,     SLOT(setUserAnswer(QString)));
    connect(d->answerWidget, SIGNAL(commitAnswer(QString)),
            d->examinator,     SLOT(processAnswerEarly()));
    connect(d->answerWidget, SIGNAL(userEvent(int, int)),
            d->examinator,   SIGNAL(userEvent(int, int)));

    setExaminatorState( d->examinator->state() );
}

/*!
 Deletes the object.
 */
ExamineWidget::~ExamineWidget()
{
    qDebug() << "~ExamineWidget()";
    delete d;
}

void ExamineWidget::resizeEvent(QResizeEvent * re)
{
    QWidget::resizeEvent(re);

    QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, re->size().height()));
    linearGradient.setColorAt(0, "#E6E6E6");
    linearGradient.setColorAt(1, "#CCCCCC");

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(linearGradient));
    setAutoFillBackground(true);
    setPalette(palette);
}

void ExamineWidget::setTaskName(QString name)
{
    if ( name.length() >50 )
        name = name.left(40)+"...";

    d->taskName = name;
    d->l_processing->setText(tr("Processing...\n%1").arg(d->taskName));
}

void ExamineWidget::setExaminatorMode(Examinator::Mode mode)
{
    d->answerWidget->setExaminatorMode(mode);
}

void ExamineWidget::setExaminatorState(Examinator::State s)
{
    d->answerWidget->setExaminatorState(s);

    if ( s == Examinator::Processing ||
         s == Examinator::IndicatingMismatch ||
         s == Examinator::IndicatingMatch) {
        d->b_play->setPixmap( QPixmap(":/images/button-pause.png"));
        d->b_play->setPixmapHl( QPixmap(":/images/button-pause.png"));
        d->b_play->setPixmapDown( QPixmap(":/images/button-pause-pressed.png"));
        d->b_play->update();

        disconnect( d->b_play, SIGNAL(clicked()),0,0);
        connect( d->b_play, SIGNAL(clicked()), 
                 d->examinator, SLOT(pause()));

        d->l_processing->setText(tr("Processing...\n%1").arg(d->taskName));
    }
    else {
        d->b_play->setPixmap( QPixmap(":/images/button-play.png"));
        d->b_play->setPixmapHl( QPixmap(":/images/button-play.png"));
        d->b_play->setPixmapDown( QPixmap(":/images/button-play-pressed.png"));
        d->b_play->update();

        disconnect( d->b_play, SIGNAL(clicked()),0,0);
        connect( d->b_play, SIGNAL(clicked()), 
                 d->examinator, SLOT(continuePlay()));

        d->l_processing->setText(tr("Paused\n%1").arg(d->taskName));
    }
}

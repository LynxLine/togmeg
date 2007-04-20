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
    Examinator * examinator;
};

/*!
 Crestes ExamineWidget
 */
ExamineWidget::ExamineWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->setMargin(20);
    vlayout->setSpacing(30);
    layout->addLayout( vlayout );

    //question

    QHBoxLayout * questionLayout = new QHBoxLayout;
    questionLayout->setSpacing(0);
    questionLayout->setMargin(0);
    vlayout->addLayout( questionLayout, 60 );

    QWidget * space1 = new QWidget;
    QWidget * space2 = new QWidget;

    d->questionWidget = new QuestionWidget;

    questionLayout->addWidget(space1,            12);
    questionLayout->addWidget(d->questionWidget, 75);
    questionLayout->addWidget(space2,            12);

    //answer

    QHBoxLayout * answerLayout = new QHBoxLayout;
    answerLayout->setSpacing(0);
    answerLayout->setMargin(0);
    vlayout->addLayout( answerLayout, 40 );

    QWidget * space3 = new QWidget;
    QWidget * space4 = new QWidget;

    d->answerWidget = new AnswerWidget;

    answerLayout->addWidget(space3,          25);
    answerLayout->addWidget(d->answerWidget, 50);
    answerLayout->addWidget(space4,          25);

    //progress
    /*
    QProgressBar * pb_time = new QProgressBar;
    pb_time->setTextVisible(false);
    pb_time->setRange(0,100);
    pb_time->setValue(0);
    layout->addWidget(pb_time);
    */
    
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

    PixmapButton * b_play = new PixmapButton(
            ":/images/button-pause.png",
            ":/images/button-pause.png",
            ":/images/button-pause-pressed.png"
        );

    QLabel * l_processing = new QLabel(tr("Processing...\nTest Name, X Entries"));

    footerLayout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum),0,0);
    footerLayout->addWidget(b_play, 0,1);
    footerLayout->addWidget(l_processing, 0,2);
    footerLayout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum),0,3);

    footerLayout->setColumnStretch(0,10);
    footerLayout->setColumnStretch(2,80);
    footerLayout->setColumnStretch(3,10);

    //examinator
    d->examinator = new Examinator(this);
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

Examinator * ExamineWidget::examinator()
{
    return d->examinator;
}

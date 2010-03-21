//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "answerwidget.h"
#include "mainwindow.h"

class AnswerWidget::Private {
public:
    QString answer;
    AnswerLineEdit * le_answer;
    Examinator::Mode mode;
};

/*!
 * Creates the object.
 */
AnswerWidget::AnswerWidget(QWidget * parent)
:RoundedGradientWidget(parent)
{
    d = new Private;

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setMargin(20);
    layout->setSpacing(0);
    setLayout(layout);

    setFont( MainWindow::baseFont(3, QFont::Bold) );

    d->le_answer = new AnswerLineEdit;
    d->le_answer->setAlignment(Qt::AlignCenter);

    {
        QFontMetrics fm(font()); 
        d->le_answer->setMinimumHeight(fm.height()+6);
        QPalette p = d->le_answer->palette();
        p.setColor(QPalette::Text, QColor("#0000ff"));
        d->le_answer->setPalette(p);
    }
    
    connect(d->le_answer, SIGNAL(returnPressed()),
            this,           SLOT(returnPressed()));
    connect(d->le_answer, SIGNAL(textChanged(const QString &)),
            this,           SLOT(textChanged(const QString &)));
    connect(d->le_answer, SIGNAL(userEvent(int, int)),
            this,         SIGNAL(userEvent(int, int)));

    setFocusProxy( d->le_answer );
    layout->addWidget(d->le_answer);

    setMaximumHeight(150);
    setMinimumHeight(150);
    setWindowTitleFont( MainWindow::baseFont(1.1, QFont::Bold) );
    setWindowTitle(tr("Answer"));

    QLinearGradient gradient;
    gradient.setColorAt(0, "#CCCCCC");
    gradient.setColorAt(1, "#E0E0E0");
    setGradient( gradient );
}

/*!
 * Deletes it.
 */
AnswerWidget::~AnswerWidget()
{
    delete d;
}

void AnswerWidget::paintEvent(QPaintEvent * pe)
{
    RoundedGradientWidget::paintEvent(pe);

    QPainter p(this);
    p.setClipRegion(pe->region());
    p.setRenderHint(QPainter::Antialiasing);

    qreal w = qreal(width());
    qreal h = qreal(height());

    QFont f = MainWindow::baseFont(0.93);
    f.setItalic(true);
    QFontMetrics fm(f);

    qreal fh = fm.height()+2.0;
    
    p.setFont( f );
    p.setPen(QPen(QColor("#666666"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.drawText(QRect(0,h-fh,w-20,fh), 
        Qt::AlignVCenter | Qt::AlignRight, 
        tr("Press Enter for next question...")
    );
}

QString AnswerWidget::answer()
{
    return d->le_answer->text();
}

void AnswerWidget::setAnswer(QString a)
{
    d->answer = a;
    if ( d->mode == Examinator::Playing ) {
        d->le_answer->clear();
        d->le_answer->setText(a);
    }
    else
        d->le_answer->clear();
}

void AnswerWidget::textChanged(const QString & text)
{
    emit userAnswerChanged( text );
}

void AnswerWidget::returnPressed()
{
    emit commitAnswer( answer() );
}

void AnswerWidget::setExaminatorMode(Examinator::Mode mode)
{
    if ( mode == Examinator::Playing ) {
        d->le_answer->setFrame(false);
        d->le_answer->setReadOnly(true);

        QPalette palette = d->le_answer->palette();
        palette.setColor(QPalette::Base, QColor(0,0,0,0));
        d->le_answer->setPalette(palette);
    }
    else {
        d->le_answer->setFrame(true);
        d->le_answer->setReadOnly(false);

        QPalette palette = d->le_answer->palette();
        palette.setColor(QPalette::Base, "#FFFFFF");
        d->le_answer->setPalette(palette);
    }
    d->mode = mode;
}

void AnswerWidget::setExaminatorState(Examinator::State s)
{
    //qDebug() << "AnswerWidget::setExaminatorState()" << s;
    if (s == Examinator::IndicatingMatch) {
        d->le_answer->setText( d->answer );
        d->le_answer->setReadOnly(true);
        d->le_answer->setFrame(false);

        QPalette palette = d->le_answer->palette();
        palette.setColor(QPalette::Base, QColor(0,0,0,0));
        d->le_answer->setPalette(palette);

        QLinearGradient gradient;
        gradient.setColorAt(0, "#B0FFB0");
        gradient.setColorAt(1, "#B0FFB0");
        setGradient( gradient );
    }
    else if (s == Examinator::IndicatingMismatch) {
        d->le_answer->setText( d->answer );
        d->le_answer->setReadOnly(true);
        d->le_answer->setFrame(false);

        QPalette palette = d->le_answer->palette();
        palette.setColor(QPalette::Base, QColor(0,0,0,0));
        d->le_answer->setPalette(palette);    

        QLinearGradient gradient;
        gradient.setColorAt(0, "#FFA0A0");
        gradient.setColorAt(1, "#FFA0A0");
        setGradient( gradient );
    }
    else {
        d->le_answer->setFrame( d->mode != Examinator::Playing );
        d->le_answer->setReadOnly( d->mode == Examinator::Playing );

        QPalette palette = d->le_answer->palette();
        palette.setColor(QPalette::Base, 
            d->mode == Examinator::Playing ? QColor(0,0,0,0) : "#FFFFFF"
        );
        d->le_answer->setPalette(palette);

        QLinearGradient gradient;
        gradient.setColorAt(0, "#CCCCCC");
        gradient.setColorAt(1, "#E0E0E0");
        setGradient( gradient );
    }

    update();
}

//
// AnswerLineEdit
//

class AnswerLineEdit::Private {
public:
    QTime startTime;
    bool firstKey;
};

AnswerLineEdit::AnswerLineEdit(QWidget * parent)
:QLineEdit(parent)
{
    d = new Private;
    reset();
}

AnswerLineEdit::~AnswerLineEdit()
{
    delete d;
}

void AnswerLineEdit::reset()
{
    clear();
    d->firstKey = true;
    d->startTime = QTime::currentTime();
}

void AnswerLineEdit::keyPressEvent(QKeyEvent * ke)
{
    QLineEdit::keyPressEvent(ke);

    int msecs = d->startTime.msecsTo( QTime::currentTime() );
    if ( d->firstKey ) {
        d->firstKey = false;
        emit userEvent(msecs, int(TypingStarted));
    }
    else
        emit userEvent(msecs, int(SymbolTyped));
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "answerwidget.h"
#include "mainwindow.h"

class AnswerWidget::Private {
public:
    QLineEdit * le_answer;
};

/*!
 * Creates the object.
 */
AnswerWidget::AnswerWidget(QWidget * parent)
:RoundedGradientWidget(parent)
{
    d = new Private;

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setMargin(50);
    layout->setSpacing(0);
    setLayout(layout);

    setFont( MainWindow::baseFont(1.5, QFont::Bold) );

    d->le_answer = new QLineEdit;
    d->le_answer->setAlignment(Qt::AlignCenter);
    connect(d->le_answer, SIGNAL(returnPressed()),
            this,           SLOT(returnPressed()));

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(d->le_answer);
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    setMaximumHeight(150);
    setWindowTitleFont( MainWindow::baseFont(1.1, QFont::Bold) );
    setWindowTitle(tr("Answer"));

    if (gradient().type() == QGradient::LinearGradient) {
        QLinearGradient * linearGradient = (QLinearGradient *)&gradient();
        linearGradient->setColorAt(0, "#CCCCCC");
        linearGradient->setColorAt(1, "#E0E0E0");
    }
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
    d->le_answer->setText(a);
}

void AnswerWidget::returnPressed()
{
    qDebug() << "returnPressed";
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
}

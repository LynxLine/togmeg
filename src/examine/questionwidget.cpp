//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "questionwidget.h"
#include "mainwindow.h"

class QuestionWidget::Private {
public:
    QPixmap questionMarkPm;
    QLabel * l_question;
};

/*!
 * Creates the object.
 */
QuestionWidget::QuestionWidget(QWidget * parent)
:RoundedGradientWidget(parent)
{
    d = new Private;
    d->questionMarkPm = QPixmap(":/images/bg-question-mark.png");

    QVBoxLayout * layout = new QVBoxLayout;
    layout->setMargin(50);
    layout->setSpacing(0);
    setLayout(layout);

    setFont( MainWindow::baseFont(1.5, QFont::Bold) );

    d->l_question = new QLabel("Test");
    d->l_question->setAlignment(Qt::AlignCenter);

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(d->l_question);
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    setWindowTitleFont( MainWindow::baseFont(1.1, QFont::Bold) );
    setWindowTitle(tr("Question"));

    if (gradient().type() == QGradient::LinearGradient) {
        QLinearGradient * linearGradient = (QLinearGradient *)&gradient();
        linearGradient->setColorAt(0, "#CCCCCC");
        linearGradient->setColorAt(1, "#E0E0E0");
    }
}

/*!
 * Deletes it.
 */
QuestionWidget::~QuestionWidget()
{
    delete d;
}

void QuestionWidget::paintEvent(QPaintEvent * pe)
{
    RoundedGradientWidget::paintEvent(pe);

    QPainter p(this);
    p.setClipRegion(pe->region());
    p.setRenderHint(QPainter::Antialiasing);

    qreal w = qreal(width());
    qreal h = qreal(height());

    p.drawPixmap(w-d->questionMarkPm.width(),
                 h-d->questionMarkPm.height(),
                 d->questionMarkPm
        );
}

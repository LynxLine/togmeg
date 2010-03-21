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
    layout->setMargin(20);
    layout->setSpacing(0);
    setLayout(layout);

    setFont( MainWindow::baseFont(3, QFont::Bold) );

    d->l_question = new QLabel;
    d->l_question->setAlignment(Qt::AlignCenter);
    d->l_question->setFont( MainWindow::baseFont(3, QFont::Bold) );

    {
        QPalette p = d->l_question->palette();
        p.setColor(QPalette::Text, QColor("#808080"));
        d->l_question->setPalette(p);
    }
    
    layout->addWidget(d->l_question);

    setMaximumHeight(150);
    setMinimumHeight(150);
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

QString QuestionWidget::question()
{
    return d->l_question->text();
}

void QuestionWidget::setQuestion(QString q)
{
    d->l_question->setText(q);
}

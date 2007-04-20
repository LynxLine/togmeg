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

        //linearGradient->setColorAt(0, "#29D22C");
        //linearGradient->setColorAt(1, "#80D280");
    }
}

/*!
 * Deletes it.
 */
AnswerWidget::~AnswerWidget()
{
    delete d;
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
    emit commitAnswer( answer() );
}

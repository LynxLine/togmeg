//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "answerwidget.h"
#include "mainwindow.h"

class AnswerWidget::Private {
public:
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

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(new QLineEdit);
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding));

    setWindowTitleFont( MainWindow::baseFont(1.5, QFont::Bold) );
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

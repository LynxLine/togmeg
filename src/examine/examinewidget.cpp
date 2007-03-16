//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "examinewidget.h"
#include "svgwidget.h"

class ExamineWidget::Private
{
public:
    SvgWidget * svgWidget;
};

/*!
 Crestes ExamineWidget
 */
ExamineWidget::ExamineWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setMargin(10);
    layout->setSpacing(5);
    setLayout(layout);

    d->svgWidget = new SvgWidget(this);
    layout->addWidget( d->svgWidget );
}

/*!
 Deletes the object.
 */
ExamineWidget::~ExamineWidget()
{
    delete d;
}

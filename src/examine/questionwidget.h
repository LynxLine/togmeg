//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include "roundedgradientwidget.h"

/*!
 * \class QuestionWidget
 */
class QuestionWidget : public RoundedGradientWidget
{
Q_OBJECT
public:
    QuestionWidget(QWidget * parent = 0);
    virtual ~QuestionWidget();

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    class Private;
    Private * d;
};

#endif // QUESTIONWIDGET_H

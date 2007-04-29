//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef ANSWERWIDGET_H
#define ANSWERWIDGET_H

#include "examinator.h"
#include "roundedgradientwidget.h"

/*!
 * \class AnswerWidget
 */
class AnswerWidget : public RoundedGradientWidget
{
Q_OBJECT
public:
    AnswerWidget(QWidget * parent = 0);
    virtual ~AnswerWidget();

    QString answer();

public slots:
    void setAnswer(QString);
    void setExaminatorMode(Examinator::Mode);

signals:
    void commitAnswer(QString);

private slots:
    void returnPressed();

protected:
    void paintEvent(QPaintEvent *);

private:
    class Private;
    Private * d;
};

#endif // ANSWERWIDGET_H

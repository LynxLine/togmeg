// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef ANSWERWINDOW_H
#define ANSWERWINDOW_H

#include "boxwindow.h"

/*!
 * \class AnswerWindow
 */
class AnswerWindow : public BoxWindow
{
Q_OBJECT
public:
    enum State { Default, Mistake };
    AnswerWindow(QWidget * parent = 0);
    ~AnswerWindow();

    QString answer();
    QString enteredAnswer();
    bool isAnswerCorrect();
    void setState(State);
    
public slots:
    void showWindow();
    void hideWindow();

    void clear();
    void setAnswer(QString);

signals:
    void returnPressed();
    
private:
    class Private;
    Private * d;
};

#endif // ANSWERWINDOW_H


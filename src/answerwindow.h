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
    AnswerWindow(QWidget * parent = 0);
    ~AnswerWindow();

    bool isAnswerCorrect();
    
public slots:
    void showWindow();
    void hideWindow();

    void setAnswer(QString);

signals:
    void returnPressed();
    
private:
    class Private;
    Private * d;
};

#endif // ANSWERWINDOW_H


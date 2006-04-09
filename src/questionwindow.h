// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include "boxwindow.h"

/*!
 * \class QuestionWindow
 */
class QuestionWindow : public BoxWindow
{
Q_OBJECT
public:
    QuestionWindow(QWidget * parent = 0);
    ~QuestionWindow();

public slots:
    void showWindow();
    void hideWindow();

private:
    class Private;
    Private * d;
};

#endif // QUESTIONWINDOW_H


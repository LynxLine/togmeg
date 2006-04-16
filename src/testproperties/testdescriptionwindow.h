// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#ifndef TESTDESCRIPTIONWINDOW_H
#define TESTDESCRIPTIONWINDOW_H

#include "boxwindow.h"

/*!
 * \class TestDescriptionWindow
 */
class TestDescriptionWindow : public BoxWindow
{
Q_OBJECT
public:
    TestDescriptionWindow(QWidget * parent = 0);
    ~TestDescriptionWindow();

public slots:
    void showWindow();
    void hideWindow();

private slots:
    void newTest();
    void loadData();

protected:
    virtual void keyPressEvent(QKeyEvent *);

private:
    class Private;
    Private * d;
};

#endif // TESTDESCRIPTIONWINDOW_H


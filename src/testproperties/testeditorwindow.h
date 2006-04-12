// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#ifndef TESTEDITORWINDOW_H
#define TESTEDITORWINDOW_H

#include "boxwindow.h"

/*!
 * \class TestEditorWindow
 */
class TestEditorWindow : public BoxWindow
{
Q_OBJECT
public:
    TestEditorWindow(QWidget * parent = 0);
    ~TestEditorWindow();

public slots:
    void showWindow();
    void hideWindow();

private slots:
    void loadData();

protected:
    virtual void keyPressEvent(QKeyEvent * ke);

private:
    class Private;
    Private * d;
};

#endif // TESTEDITORWINDOW_H


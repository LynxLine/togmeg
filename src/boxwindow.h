// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef BOXWINDOW_H
#define BOXWINDOW_H

#include "movingwidget.h"

/*!
 \class BoxWindow
 */
class BoxWindow : public MovingWidget
{
Q_OBJECT
public:
    BoxWindow(QWidget * parent = 0);
    ~BoxWindow();

protected:
    virtual void paintEvent(QPaintEvent * pe);
    
private:
    class Private;
    Private * d;
};

#endif // BOXWINDOW_H


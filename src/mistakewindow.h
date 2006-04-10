// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef MISTAKEWINDOW_H
#define MISTAKEWINDOW_H

#include "movingwidget.h"

/*!
 * \class MistakeWindow
 */
class MistakeWindow : public MovingWidget
{
Q_OBJECT
public:
    MistakeWindow(QWidget * parent = 0);
    ~MistakeWindow();
    
public slots:
    void setText(QString);
    
protected:
    virtual void paintEvent(QPaintEvent * pe);

private:
    class Private;
    Private * d;
};

#endif // MISTAKEWINDOW_H


// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef TIMEOUTPANEL_H
#define TIMEOUTPANEL_H

#include "movingwidget.h"

/*!
 * \class TimeoutPanel
 */
class TimeoutPanel : public MovingWidget
{
Q_OBJECT
public:
    TimeoutPanel(QWidget * parent = 0);
    ~TimeoutPanel();

public slots:
    void showWindow();
    void hideWindow();

private:
    class Private;
    Private * d;
};

#endif // TIMEOUTPANEL_H


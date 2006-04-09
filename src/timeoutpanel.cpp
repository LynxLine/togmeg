// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "timeoutpanel.h"

class TimeoutPanel::Private {
public:
    Private(TimeoutPanel * p):self(p) 
    {;}

    TimeoutPanel * self;
};

/*!
 * Creates the object.
 */
TimeoutPanel::TimeoutPanel(QWidget * parent)
:MovingWidget(parent)
{
    d = new Private(this);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = width;
    int h = (height* 50) / 600;
    resize(w,h);
    
    int hideY = height;
    int showY = (height *550) / 600;
    int x     = 0;

    setMovingTime(100);
    setMovingEffect(QPoint(x, hideY), QPoint(x, showY));
    move(x, hideY);

    {
        QPalette pal = palette();
	    pal.setBrush( backgroundRole(), QBrush(QColor("#ffffd0")) );
        setAutoFillBackground(true);
        setPalette( pal );
    }
}

/*!
 * Deletes it.
 */
TimeoutPanel::~TimeoutPanel()
{
    delete d;
}

void TimeoutPanel::showWindow()
{
    moveForward();
}

void TimeoutPanel::hideWindow()
{
    moveBackward();
}


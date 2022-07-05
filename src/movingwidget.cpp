// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "movingwidget.h"

class MovingWidget::Private {
public:
    enum Moving { Showing, Hiding, Stop };
    Private(MovingWidget * p)
    :self(p),
     movingMsecs(180)
    {;}

    MovingWidget * self;
    
    int showX;
    int showY;
    int hideX;
    int hideY;
    
    Moving moving;
    int movingPercent;
    QTime movingStartTime;
    int movingMsecs;
};

/*!
 * Creates the object.
 */
MovingWidget::MovingWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private(this);
}

/*!
 * Deletes it.
 */
MovingWidget::~MovingWidget()
{
    delete d;
}

void MovingWidget::setMovingTime(int msecs)
{
    d->movingMsecs = msecs;
}

void MovingWidget::setMovingEffect(QPoint from, QPoint to)
{
    d->hideX = from.x();
    d->hideY = from.y();
    d->showX = to.x();
    d->showY = to.y();
}

void MovingWidget::moveForward()
{
    d->movingPercent = 0;
    d->moving = Private::Showing;
    d->movingStartTime = QTime::currentTime();
    QTimer::singleShot(0, this, SLOT(moving()));
}

void MovingWidget::moveBackward()
{
    d->movingPercent = 0;
    d->moving = Private::Hiding;
    d->movingStartTime = QTime::currentTime();
    QTimer::singleShot(0, this, SLOT(moving()));
}

void MovingWidget::moving()
{
    int x = this->x();
    int y = this->y();

    int msecs = d->movingStartTime.msecsTo(QTime::currentTime());
    d->movingPercent = msecs * 100 / d->movingMsecs; 
    
    if ( d->movingPercent > 100) {
        if (d->moving == Private::Hiding)  {
            x = d->hideX;
            y = d->hideY;
        }
        if (d->moving == Private::Showing) {
            x = d->showX;
            y = d->showY;
        }
        d->moving = Private::Stop;
    }
    else {
        if (d->moving == Private::Hiding)  {
            x = d->hideX + ((d->showX - d->hideX) * (100 - d->movingPercent)) /100;
            y = d->hideY + ((d->showY - d->hideY) * (100 - d->movingPercent)) /100;
        }
        if (d->moving == Private::Showing) {
            x = d->hideX + ((d->showX - d->hideX) * d->movingPercent) /100;
            y = d->hideY + ((d->showY - d->hideY) * d->movingPercent) /100;
        }
    }
    
    move(x, y);
    if ( d->moving != Private::Stop) QTimer::singleShot(1, this, SLOT(moving()));
}


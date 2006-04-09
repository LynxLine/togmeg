// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "boxwindow.h"
#define SCROLL_MSECS 200

class BoxWindow::Private {
public:
    enum Moving { Showing, Hiding, Stop };
    Private(BoxWindow * p):
        pmTop        (":/images/yellow-box/top.png"),
        pmLeft       (":/images/yellow-box/left.png"),
        pmRight      (":/images/yellow-box/right.png"),
        pmBottom     (":/images/yellow-box/bottom.png"),
        pmLeftTop    (":/images/yellow-box/top-left.png"),
        pmRightTop   (":/images/yellow-box/top-right.png"),
        pmLeftBottom (":/images/yellow-box/bottom-left.png"),
        pmRightBottom(":/images/yellow-box/bottom-right.png"),
        self(p)
    {}

    QPixmap pmTop;
    QPixmap pmLeft;
    QPixmap pmRight;
    QPixmap pmBottom;
    QPixmap pmLeftTop;
    QPixmap pmRightTop;
    QPixmap pmLeftBottom;
    QPixmap pmRightBottom;

    BoxWindow * self;
    
    int showX;
    int showY;
    int hideX;
    int hideY;
    
    Moving moving;
    int movingPercent;
    QTime movingStartTime;
    
    void calculateSize();
};

/*!
 * Creates the object.
 */
BoxWindow::BoxWindow(QWidget * parent)
:QWidget(parent)
{
    d = new Private(this);
    
    setContentsMargins(d->pmLeftTop.width(),     d->pmLeftTop.height(), 
                       d->pmRightBottom.width(), d->pmRightBottom.height());
    
    setAutoFillBackground(true);

    setMinimumWidth ( d->pmLeftTop.width() +1+d->pmRightBottom.width() );
    setMinimumHeight( d->pmLeftTop.height()+1+d->pmRightBottom.height() );
}

/*!
 * Deletes it.
 */
BoxWindow::~BoxWindow()
{
    delete d;
}

void BoxWindow::paintEvent(QPaintEvent * pe)
{
    int w = width();
    int h = height();

    int w1 = d->pmLeftTop.width();
    int w2 = d->pmRightBottom.width();
    
    int h1 = d->pmLeftTop.height();
    int h2 = d->pmRightBottom.height();
    
    QPainter p(this);
    p.setClipRegion( pe->region() );
    
    p.drawPixmap(0,    0,    w1, h1, d->pmLeftTop);
    p.drawPixmap(0,    h-h2, w1, h2, d->pmLeftBottom);
    p.drawPixmap(w-w2, 0,    w2, h1, d->pmRightTop);
    p.drawPixmap(w-w2, h-h2, w2, h2, d->pmRightBottom);

    p.drawPixmap(0,    h1,   w1,      h-h1-h2, d->pmLeft);
    p.drawPixmap(w1,   0,    w-w1-w2, h1,      d->pmTop);
    p.drawPixmap(w-w2, h1,   w2,      h-h1-h2, d->pmRight);
    p.drawPixmap(w1,   h-h2, w-w1-w2, h2,      d->pmBottom);
    
    p.fillRect(w1,h1, w-w1-w2, h-h1-h2, QColor("#ffffd0"));
    p.end();
    
    QWidget::paintEvent(pe);
}

void BoxWindow::setMovingEffect(QPoint from, QPoint to)
{
    d->hideX = from.x();
    d->hideY = from.y();
    d->showX = to.x();
    d->showY = to.y();
}

void BoxWindow::moveForward()
{
    d->movingPercent = 0;
    d->moving = Private::Showing;
    d->movingStartTime = QTime::currentTime();
    QTimer::singleShot(0, this, SLOT(moving()));
}

void BoxWindow::moveBackward()
{
    d->movingPercent = 0;
    d->moving = Private::Hiding;
    d->movingStartTime = QTime::currentTime();
    QTimer::singleShot(0, this, SLOT(moving()));
}

void BoxWindow::moving()
{
    int x = this->x();
    int y = this->y();

    int msecs = d->movingStartTime.msecsTo(QTime::currentTime());
    d->movingPercent = msecs * 100 / SCROLL_MSECS; 
    
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
            y = d->hideX + ((d->showX - d->hideX) * d->movingPercent) /100;
            y = d->hideY + ((d->showY - d->hideY) * d->movingPercent) /100;
        }
    }
    
    move( x, y);
    if ( d->moving != Private::Stop) QTimer::singleShot(1, this, SLOT(moving()));
}


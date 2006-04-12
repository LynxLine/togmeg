// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "boxwindow.h"
#define SCROLL_MSECS 200

class BoxWindow::Private {
public:
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
    
};

/*!
 * Creates the object.
 */
BoxWindow::BoxWindow(QWidget * parent)
:MovingWidget(parent)
{
    d = new Private(this);
    setAutoFillBackground(true);
    setStyle(Round25);
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

void BoxWindow::setStyle(BoxWindow::Style style)
{
    if (style == Round25) {
        d->pmTop        = QPixmap(":/images/yellow-box/top.png");
        d->pmLeft       = QPixmap(":/images/yellow-box/left.png");
        d->pmRight      = QPixmap(":/images/yellow-box/right.png");
        d->pmBottom     = QPixmap(":/images/yellow-box/bottom.png");
        d->pmLeftTop    = QPixmap(":/images/yellow-box/top-left.png");
        d->pmRightTop   = QPixmap(":/images/yellow-box/top-right.png");
        d->pmLeftBottom = QPixmap(":/images/yellow-box/bottom-left.png");
        d->pmRightBottom= QPixmap(":/images/yellow-box/bottom-right.png");
    }
    else if (style == Round10) {
        d->pmTop        = QPixmap(":/images/yellow-small-box/top.png");
        d->pmLeft       = QPixmap(":/images/yellow-small-box/left.png");
        d->pmRight      = QPixmap(":/images/yellow-small-box/right.png");
        d->pmBottom     = QPixmap(":/images/yellow-small-box/bottom.png");
        d->pmLeftTop    = QPixmap(":/images/yellow-small-box/top-left.png");
        d->pmRightTop   = QPixmap(":/images/yellow-small-box/top-right.png");
        d->pmLeftBottom = QPixmap(":/images/yellow-small-box/bottom-left.png");
        d->pmRightBottom= QPixmap(":/images/yellow-small-box/bottom-right.png");
    }
    
    setContentsMargins(d->pmLeftTop.width(),     d->pmLeftTop.height(), 
                       d->pmRightBottom.width(), d->pmRightBottom.height());

    setMinimumWidth ( d->pmLeftTop.width() +1+d->pmRightBottom.width() );
    setMinimumHeight( d->pmLeftTop.height()+1+d->pmRightBottom.height() );
}


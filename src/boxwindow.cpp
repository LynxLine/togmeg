// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "boxwindow.h"
#define SCROLL_MSECS 200

class BoxWindow::Private {
public:
    Private(BoxWindow * p):
        self(p)
    {
    }

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
    
    QPalette pal = palette();
    pal.setBrush( QPalette::Window, QBrush(QColor("#ffffd0")) );
    setPalette( pal );
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

    QWidget::paintEvent(pe);
    
    int b =5;

    QPainter p(this);
    p.setClipRegion( pe->region() );
    
    p.fillRect(0,   0,   w, b, QColor("#ff7030") );
    p.fillRect(0,   h-b, w, b, QColor("#ff7030") );
    p.fillRect(0,   0,   b, h, QColor("#ff7030") );
    p.fillRect(w-b, 0,   b, h, QColor("#ff7030") );
    
    p.end();
}


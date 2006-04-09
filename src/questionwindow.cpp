// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "questionwindow.h"

#define SCROLL_MSECS 200

class QuestionWindow::Private {
public:
    enum Moving { Showing, Hiding, Stop };
    Private(QuestionWindow * p):self(p) 
    {
        calculateSize();
    }

    QSize size;
    QuestionWindow * self;
    
    int x;
    int showY;
    int hideY;
    
    Moving moving;
    int movingPercent;
    QTime movingStartTime;
    
    void calculateSize();
};

/*!
 * Creates the object.
 */
QuestionWindow::QuestionWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private(this);
    
    setVisible(false);
    move(d->x, d->hideY);
}

/*!
 * Deletes it.
 */
QuestionWindow::~QuestionWindow()
{
    delete d;
}

void QuestionWindow::Private::calculateSize()
{
    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 600) / 800;
    int h = (height* 230) / 600;

    size = QSize(w,h);
    self->resize(size);
    
    hideY = -h;
    showY = (height * 50) / 600;
    x = (width * 100) / 800;
}

void QuestionWindow::showEvent(QShowEvent * se)
{
    BoxWindow::showEvent(se);

    d->movingPercent = 0;
    d->moving = Private::Showing;
    d->movingStartTime = QTime::currentTime();
    QTimer::singleShot(0, this, SLOT(moving()));
}

void QuestionWindow::moving()
{
    int y = this->y();

    int msecs = d->movingStartTime.msecsTo(QTime::currentTime());
    d->movingPercent = msecs * 100 / SCROLL_MSECS; 

    if ( d->movingPercent > 100) {
        if (d->moving == Private::Hiding)  y = d->hideY;
        if (d->moving == Private::Showing) y = d->showY;
        d->moving = Private::Stop;
    }
    else {
        if (d->moving == Private::Hiding)  y = d->hideY + ((d->showY - d->hideY) * (100 - d->movingPercent)) /100;
        if (d->moving == Private::Showing) y = d->hideY + ((d->showY - d->hideY) * d->movingPercent) /100;
    }
    
    move( d->x, y);
    if ( d->moving != Private::Stop) QTimer::singleShot(1, this, SLOT(moving()));
}


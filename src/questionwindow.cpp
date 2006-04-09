// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "questionwindow.h"

#define SCROLL_MSECS 200

class QuestionWindow::Private {
public:
    Private(QuestionWindow * p):self(p) 
    {;}

    QuestionWindow * self;
};

/*!
 * Creates the object.
 */
QuestionWindow::QuestionWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private(this);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 600) / 800;
    int h = (height* 230) / 600;
    resize(w,h);
    
    int hideY = -h;
    int showY = (height * 50) / 600;
    int x     = (width * 100) / 800;
    setMovingEffect(QPoint(x, hideY), QPoint(x, showY));
    move(x, hideY);
}

/*!
 * Deletes it.
 */
QuestionWindow::~QuestionWindow()
{
    delete d;
}

void QuestionWindow::showWindow()
{
    moveForward();
}

void QuestionWindow::hideWindow()
{
    moveBackward();
}


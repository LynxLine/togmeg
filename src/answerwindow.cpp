// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "answerwindow.h"

#define SCROLL_MSECS 200

class AnswerWindow::Private {
public:
    Private(AnswerWindow * p):self(p) 
    {;}

    AnswerWindow * self;
};

/*!
 * Creates the object.
 */
AnswerWindow::AnswerWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private(this);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 600) / 800;
    int h = (height*  50) / 600;
    resize(w,h);
    
    int hideY = height;
    int showY = (height * 400) / 600;
    int x     = (width * 100) / 800;

    setMovingEffect(QPoint(x, hideY), QPoint(x, showY));
    move(x, hideY);
}

/*!
 * Deletes it.
 */
AnswerWindow::~AnswerWindow()
{
    delete d;
}

void AnswerWindow::showWindow()
{
    moveForward();
}

void AnswerWindow::hideWindow()
{
    moveBackward();
}


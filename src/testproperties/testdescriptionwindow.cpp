// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include "testdescriptionwindow.h"

class TestDescriptionWindow::Private {
public:
};

/*!
 * Creates the object.
 */
TestDescriptionWindow::TestDescriptionWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private;
    setStyle(Round10);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 385) / 800;
    int h = (height* 250) / 600;
    resize(w,h);
    
    int hideX = -w;
    int y         = (height * 50) / 600;
    int showX     = (width *  10) / 800;
    setMovingEffect(QPoint(hideX, y), QPoint(showX, y));
    move(hideX, y);

    //int b = (height*  10) / 600;
}

/*!
 * Deletes it.
 */
TestDescriptionWindow::~TestDescriptionWindow()
{
    delete d;
}

void TestDescriptionWindow::showWindow()
{
    moveForward();
}

void TestDescriptionWindow::hideWindow()
{
    moveBackward();
}


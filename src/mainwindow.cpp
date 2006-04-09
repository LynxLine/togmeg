// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"

class MainWindow::Private
{
public:
    Private(MainWindow * mw) {self=mw;}
    static MainWindow * self;
};

MainWindow * MainWindow::Private::self = 0L;

/*!
 Creates MainWindow object.
 */
MainWindow::MainWindow(QWidget* parent, Qt::WFlags f)
:QMainWindow(parent, f)
{
    d = new Private(this);

    setWindowTitle("aLesson");

    // qsettings stuff
    QCoreApplication::setOrganizationName("UAELTechnology");
    QCoreApplication::setOrganizationDomain("uael.com.ua");
    QCoreApplication::setApplicationName("aLesson");

	statusBar()->hide();
    menuBar()->hide();

    showFullScreen();
}

/*!
 Deletes the object.
 */
MainWindow::~MainWindow()
{
    delete d;
}

/*!
 Returns the MainWindow object.
 Useful for accessing from other classes.
 */
MainWindow * MainWindow::self()
{
    return MainWindow::Private::self;
}


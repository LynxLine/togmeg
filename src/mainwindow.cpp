// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"
#include "timeoutpanel.h"
#include "answerwindow.h"
#include "questionwindow.h"

class MainWindow::Private
{
public:
    Private(MainWindow * mw) {self=mw;}
    static MainWindow * self;

    TimeoutPanel * timeoutPanel;
    AnswerWindow * answerWindow;
    QuestionWindow * questionWindow;
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

    {
        QPalette pal = palette();
	    pal.setBrush( backgroundRole(), QBrush(QColor("#888")) );
        setPalette( pal );
    }
    
	statusBar()->hide();
    menuBar()->hide();

    d->timeoutPanel = new TimeoutPanel(this);
    d->answerWindow = new AnswerWindow(this);
    d->questionWindow = new QuestionWindow(this);
    
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

void MainWindow::showEvent(QShowEvent * se)
{
    QMainWindow::showEvent(se);
    QTimer::singleShot(200, d->timeoutPanel, SLOT(showWindow()));
    QTimer::singleShot(100, d->answerWindow, SLOT(showWindow()));
    QTimer::singleShot(100, d->questionWindow, SLOT(showWindow()));
}

void MainWindow::mousePressEvent(QMouseEvent * me)
{
    QMainWindow::mousePressEvent(me);
    
    QTimer::singleShot( 10, d->timeoutPanel, SLOT(hideWindow()));
    QTimer::singleShot(100, d->answerWindow, SLOT(hideWindow()));
    QTimer::singleShot(100, d->questionWindow, SLOT(hideWindow()));
    QTimer::singleShot(300, qApp, SLOT(quit()));
}


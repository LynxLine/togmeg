// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"
#include "timeoutpanel.h"
#include "answerwindow.h"
#include "mistakewindow.h"
#include "questionwindow.h"
#include "studyprocessor.h"

class MainWindow::Private
{
public:
    Private(MainWindow * mw) {self=mw;}
    static MainWindow * self;

    TimeoutPanel * timeoutPanel;
    AnswerWindow * answerWindow;
    MistakeWindow * mistakeWindow;
    QuestionWindow * questionWindow;

    StudyProcessor * studyProcessor;
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
    d->mistakeWindow = new MistakeWindow(this);
    d->questionWindow = new QuestionWindow(this);
    
    d->studyProcessor = new StudyProcessor(this);
    
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
    QTimer::singleShot(1000, d->studyProcessor, SLOT(start()));
}

void MainWindow::mousePressEvent(QMouseEvent * me)
{
    QMainWindow::mousePressEvent(me);
    d->studyProcessor->stop();
    
    QTimer::singleShot(1000, qApp, SLOT(quit()));
}

TimeoutPanel * MainWindow::timeoutPanel()
{
    return d->timeoutPanel;
}

AnswerWindow * MainWindow::answerWindow()
{
    return d->answerWindow;
}

MistakeWindow * MainWindow::mistakeWindow()
{
    return d->mistakeWindow;
}

QuestionWindow * MainWindow::questionWindow()
{
    return d->questionWindow;
}


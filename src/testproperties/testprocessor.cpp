// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include "mainwindow.h"
#include "testprocessor.h"
#include "testeditorwindow.h"
#include "testdescriptionwindow.h"

class TestProcessor::Private {
public:
    Private(MainWindow * p):mainWindow(p) {;}

    MainWindow * mainWindow;
    QPushButton * startButton;
    QPushButton * exitButton;
};

/*!
 * Creates the object.
 */
TestProcessor::TestProcessor(MainWindow * parent)
:QObject(parent)
{
    d = new Private(parent);

    connect(d->mainWindow->testDescriptionWindow(), SIGNAL(testSelected(int)),
            d->mainWindow->testEditorWindow(),        SLOT(loadData(int)));

    QWidget *desktop = qApp->activeWindow();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 150) / 800;
    int h = (height*  40) / 600;

    int x = (width * 125) / 800;
    int y = (height* 350) / 600;

    int b = (height*  10) / 600;
    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (b * 100 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    d->startButton = new QPushButton(tr("Start Testing"), d->mainWindow);
    d->startButton->setFont(font);
    d->startButton->resize(w,h);
    d->startButton->move(x,y);
    d->startButton->hide();

    d->exitButton = new QPushButton(tr("EXIT"), d->mainWindow);
    d->exitButton->setFont(font);
    d->exitButton->resize(w,h);
    d->exitButton->move(x,y + (height*50)/600);
    d->exitButton->hide();
    
    connect(d->exitButton, SIGNAL(clicked()), this, SLOT(exit()));
    connect(d->startButton, SIGNAL(clicked()), this, SLOT(startTesting()));
}

/*!
 * Deletes it.
 */
TestProcessor::~TestProcessor()
{
    delete d;
}

void TestProcessor::start()
{
    QTimer::singleShot(500, d->mainWindow->testEditorWindow(), SLOT(showWindow()));
    QTimer::singleShot(500, d->mainWindow->testDescriptionWindow(), SLOT(showWindow()));
    QTimer::singleShot(900, d->startButton, SLOT(show()));
    QTimer::singleShot(900, d->exitButton, SLOT(show()));
    d->mainWindow->testDescriptionWindow()->setFocus();
}

void TestProcessor::startTesting()
{
    if (d->mainWindow->viewMode() == MainWindow::TestPropertiesMode) 
        d->mainWindow->setViewMode(MainWindow::StudyingMode);
}

void TestProcessor::stop()
{
    d->exitButton->hide();
    d->startButton->hide();
    d->mainWindow->testEditorWindow()->submitAll();
    QTimer::singleShot(0, d->mainWindow->testEditorWindow(), SLOT(hideWindow()));
    QTimer::singleShot(0, d->mainWindow->testDescriptionWindow(), SLOT(hideWindow()));
}

void TestProcessor::exit()
{
    stop();
    QTimer::singleShot(500, qApp, SLOT(quit()));
}


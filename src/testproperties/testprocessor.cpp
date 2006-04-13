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
};

/*!
 * Creates the object.
 */
TestProcessor::TestProcessor(MainWindow * parent)
:QObject(parent)
{
    d = new Private(parent);
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
}

void TestProcessor::stop()
{
    d->mainWindow->testEditorWindow()->submitAll();
    QTimer::singleShot(0, d->mainWindow->testEditorWindow(), SLOT(hideWindow()));
    QTimer::singleShot(0, d->mainWindow->testDescriptionWindow(), SLOT(hideWindow()));
}


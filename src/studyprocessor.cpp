// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "mainwindow.h"
#include "studystorage.h"
#include "studyprocessor.h"

#include "timeoutpanel.h"
#include "answerwindow.h"
#include "questionwindow.h"

class StudyProcessor::Private {
public:
    Private(MainWindow * p):mainWindow(p) 
    {
        storage = new StudyStorage(p);
        currentEntry = 0L;
        timeoutMsecs = 5000;
    }
        
    MainWindow * mainWindow;
    StudyStorage * storage;
    StudyEntry * currentEntry;

    int timeoutMsecs;
    QTime progressTime;
};

/*!
 * Creates the object.
 */
StudyProcessor::StudyProcessor(MainWindow * parent)
:QObject(parent)
{
    d = new Private(parent);
}

/*!
 * Deletes it.
 */
StudyProcessor::~StudyProcessor()
{
    delete d;
}

void StudyProcessor::start()
{
    QTimer::singleShot(200, d->mainWindow->timeoutPanel(), SLOT(showWindow()));
    QTimer::singleShot(100, d->mainWindow->answerWindow(), SLOT(showWindow()));
    QTimer::singleShot(100, d->mainWindow->questionWindow(), SLOT(showWindow()));

    QTimer::singleShot(500, this, SLOT(startAsking()));
    connect(d->mainWindow->answerWindow(), SIGNAL(returnPressed()),
            this, SLOT(checkAnswer()));
}

void StudyProcessor::stop()
{
    QTimer::singleShot( 10, d->mainWindow->timeoutPanel(), SLOT(hideWindow()));
    QTimer::singleShot(100, d->mainWindow->answerWindow(), SLOT(hideWindow()));
    QTimer::singleShot(100, d->mainWindow->questionWindow(), SLOT(hideWindow()));
    disconnect(d->mainWindow->answerWindow(), SIGNAL(returnPressed()), 0L, 0L);
}

void StudyProcessor::startAsking()
{
    if (d->currentEntry) 
        d->storage->closeEntry( d->currentEntry );
    
    d->currentEntry = d->storage->nextEntry();

    d->mainWindow->questionWindow()->setQuestion( d->currentEntry->question );
    d->mainWindow->answerWindow()->setAnswer( d->currentEntry->answer );

    startProgress();
}

void StudyProcessor::startProgress()
{
    d->progressTime = QTime::currentTime();
    QTimer::singleShot(50, this, SLOT(waitingForAnswer()));
}

void StudyProcessor::waitingForAnswer()
{
    int msecs = d->progressTime.msecsTo(QTime::currentTime());
    int percent = msecs * 100 / d->timeoutMsecs;

    if (msecs < d->timeoutMsecs)
        QTimer::singleShot(50, this, SLOT(waitingForAnswer()));
    else {
        percent = 100;
        startAsking();
    }

    d->mainWindow->timeoutPanel()->setProgress(percent);
}

void StudyProcessor::checkAnswer()
{
    qDebug() << d->mainWindow->answerWindow()->isAnswerCorrect();
    startAsking();
}

StudyStorage * StudyProcessor::storage()
{
    return d->storage;
}


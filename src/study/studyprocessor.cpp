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
        timeoutMsecs = 10000;
    }
        
    MainWindow * mainWindow;
    StudyStorage * storage;
    StudyEntry * currentEntry;

    bool stopWaiting;
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
    QTimer::singleShot(600, d->mainWindow->timeoutPanel(), SLOT(showWindow()));
    QTimer::singleShot(500, d->mainWindow->answerWindow(), SLOT(showWindow()));
    QTimer::singleShot(500, d->mainWindow->questionWindow(), SLOT(showWindow()));

    QTimer::singleShot(700, this, SLOT(startAsking()));
    connect(d->mainWindow->answerWindow(), SIGNAL(returnPressed()),
            this, SLOT(returnPressed()));
}

void StudyProcessor::stop()
{
    d->stopWaiting = true;
    d->mainWindow->timeoutPanel()->setProgress(0);
    
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
    if ( !d->currentEntry ) return;

    d->mainWindow->questionWindow()->setHeader( tr("Question:") );
    d->mainWindow->questionWindow()->setQuestion( d->currentEntry->question );
    d->mainWindow->answerWindow()->setAnswer( d->currentEntry->answer );
    d->mainWindow->answerWindow()->setState(AnswerWindow::Default);
    d->mainWindow->answerWindow()->setFocus();
    d->mainWindow->answerWindow()->clear();
    
    startProgress();
}

void StudyProcessor::startProgress()
{
    d->stopWaiting = false;
    d->progressTime = QTime::currentTime();
    QTimer::singleShot(50, this, SLOT(waitingForAnswer()));
}

void StudyProcessor::waitingForAnswer()
{
    if (d->stopWaiting) {
        d->mainWindow->timeoutPanel()->setProgress(100);
        return;
    }

    int msecs = d->progressTime.msecsTo(QTime::currentTime());
    int percent = msecs * 100 / d->timeoutMsecs;

    if (msecs < d->timeoutMsecs)
        QTimer::singleShot(50, this, SLOT(waitingForAnswer()));
    else {
        percent = 100;
        checkAnswer();
    }

    d->mainWindow->timeoutPanel()->setProgress(percent);
}

void StudyProcessor::returnPressed()
{
    QString answer = d->mainWindow->answerWindow()->enteredAnswer();
    if ( answer.isEmpty() ) return;
    checkAnswer();
}

void StudyProcessor::checkAnswer()
{
    bool correct = d->mainWindow->answerWindow()->isAnswerCorrect();
    d->stopWaiting = true;

    if (correct) {
        startAsking();
    }
    else {
        QString answer = d->mainWindow->answerWindow()->answer();
    
        d->mainWindow->questionWindow()->setHeader( tr("The correct answer is:") );
        d->mainWindow->questionWindow()->setQuestion( tr("<p align='center'><font color='#080'>%1</font></p>").arg(answer) );
        d->mainWindow->answerWindow()->setState(AnswerWindow::Mistake);

        QTimer::singleShot(1500, this, SLOT(startAsking()));
    }
}

StudyStorage * StudyProcessor::storage()
{
    return d->storage;
}


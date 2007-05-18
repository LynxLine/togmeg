//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytaskmodel.h"
#include "studytaskcontroller.h"

class StudyTaskController::Private {
public:
    Private():index(0) {;}

    int index;
    QTimeLine * timeLine;
    ControllerDataEntry currentEntry;
};

/*!
 * Creates the object.
 */
StudyTaskController::StudyTaskController(StudyTaskModel * parent)
:TaskController(parent)
{
    d = new Private;

    d->timeLine = new QTimeLine(1000, this);
    d->timeLine->setCurveShape(QTimeLine::LinearCurve);
    d->timeLine->setFrameRange(0,100);
    d->timeLine->setLoopCount(1);

    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(readyForNext()), Qt::QueuedConnection);
}

/*!
 * Deletes it.
 */
StudyTaskController::~StudyTaskController()
{
    delete d;
}

bool StudyTaskController::hasNext()
{
    int nextIndex = 0;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, StudyTaskModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry StudyTaskController::next()
{
    QString question;
    while ( question.isEmpty() ) {
        d->index = rand() % model->rowCount();
        QModelIndex i = model->index(d->index, StudyTaskModel::QuestionColumn);
        question = model->data(i).toString();
    }

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index, StudyTaskModel::QuestionColumn) ).toString();
    
    entry.totalTime = 5000; //temp
    entry.startTime = 0; //temp

    d->currentEntry = entry;
 
    return entry;
}

void StudyTaskController::processAnswer(int usedTime, QString answer)
{
    if ( d->timeLine->state() != QTimeLine::NotRunning ) {
        d->timeLine->setCurrentTime( d->timeLine->duration() );
        d->timeLine->stop();
        readyForNext();
        return;
    }

    if ( d->currentEntry.answer != answer.toLower().simplified() ) {
        emit indicateMismatching();
        d->timeLine->setDuration( 1000 );
    }
    else {
        emit indicateMatching();
        d->timeLine->setDuration( 300 );
    }

    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void StudyTaskController::readyForNext()
{
    emit requestNextQuestion();
}

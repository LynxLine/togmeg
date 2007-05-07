//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytaskmodel.h"
#include "examinatetaskcontroller.h"

class ExaminateTaskController::Private {
public:
    Private():index(0) {;}

    int index;
    QTimeLine * timeLine;
    ControllerDataEntry currentEntry;
};

/*!
 * Creates the object.
 */
ExaminateTaskController::ExaminateTaskController(StudyTaskModel * parent)
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
ExaminateTaskController::~ExaminateTaskController()
{
    delete d;
}

bool ExaminateTaskController::hasNext()
{
    int nextIndex = d->index;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, StudyTaskModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry ExaminateTaskController::next()
{
    while ( d->index < model->rowCount() ) {
        QModelIndex i = model->index(d->index, StudyTaskModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        d->index++;
    }

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index, StudyTaskModel::QuestionColumn) ).toString();
    entry.msecs = 5000; //temp

    d->currentEntry = entry;
    d->index++;

    return entry;
}

void ExaminateTaskController::processAnswer(QString answer)
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

void ExaminateTaskController::readyForNext()
{
    emit requestNextQuestion();
}

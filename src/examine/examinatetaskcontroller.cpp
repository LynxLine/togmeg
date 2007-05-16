//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "crammero.h"
#include "studytask.h"
#include "studytaskmodel.h"
#include "tasklistmodel.h"
#include "examinatetaskcontroller.h"

class ExaminateTaskController::Private {
public:
    Private()/*:index(0)*/ {;}

    //int index;
    QTimeLine * timeLine;

    StudyTask * task;
    QList<int> entryIndexes;
    ControllerDataEntry currentEntry;

    bool processOnly;
    int processOnlyCount;
};

/*!
 * Creates the object.
 */
ExaminateTaskController::ExaminateTaskController(StudyTaskModel * parent)
:TaskController(parent)
{
    d = new Private;
    d->task = TaskListModel::instance()->task( model->taskId() );

    d->timeLine = new QTimeLine(1000, this);
    d->timeLine->setCurveShape(QTimeLine::LinearCurve);
    d->timeLine->setFrameRange(0,100);
    d->timeLine->setLoopCount(1);

    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(readyForNext()), Qt::QueuedConnection);

    d->processOnly = d->task->property("exam_processOnly").toBool();
    d->processOnlyCount = d->task->property("exam_processOnlyCount").toInt();

    int index = 0;
    while ( index < model->rowCount() ) {
        QModelIndex i = model->index(index, StudyTaskModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) {
            d->entryIndexes << index;
        }
        index++;
    }
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
    if ( d->processOnly && d->processOnlyCount <=0 ) return false;
    return d->entryIndexes.count() >0;
}

ControllerDataEntry ExaminateTaskController::next()
{
    int count = d->entryIndexes.count();
    int index = d->entryIndexes.first();
    bool randomize = d->task->property("exam_randomize").toBool();
    if (d->processOnly) randomize = true;
    if (randomize) {
        index = d->entryIndexes[ rand() % count ];
    }

    d->entryIndexes.removeAll(index);
    if (d->processOnly) d->processOnlyCount--;

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(index, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(index, StudyTaskModel::QuestionColumn) ).toString();
    entry.msecs = 5000; //temp

    d->currentEntry = entry;
    return entry;
}

void ExaminateTaskController::processAnswer(QString answer)
{
    //qDebug() << eventTimeMap;

    if ( d->timeLine->state() != QTimeLine::NotRunning ) {
        d->timeLine->setCurrentTime( d->timeLine->duration() );
        d->timeLine->stop();
        readyForNext();
        return;
    }

    if ( d->currentEntry.answer != answer.toLower().simplified() ) {
        d->timeLine->setDuration( 1000 );
        emit indicateMismatching();
    }
    else {
        //answer is correct
        d->timeLine->setDuration( 300 );
        emit indicateMatching();

        //commit stats
        QList<int> times = eventTimeMap.keys();
        if (times.count() >= 2) {
            qSort(times);
            int msecs = times.last() - times.first();
            if (msecs) {
                app::addTypingStats(msecs, times.count());
            }
        }
    }

    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void ExaminateTaskController::readyForNext()
{
    emit requestNextQuestion();
}

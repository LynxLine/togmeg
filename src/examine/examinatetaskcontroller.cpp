//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#ifdef Q_WS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

#include "crammero.h"
#include "studytask.h"
#include "studytaskmodel.h"
#include "tasklistmodel.h"
#include "examinatetaskcontroller.h"

class ExaminateTaskController::Private {
public:
    Private():usedTime(0) {;}

    //int index;
    QTimeLine * timeLine;

    StudyTask * task;
    QList<int> entryIndexes;
    ControllerDataEntry currentEntry;

    int usedTime;
    bool processOnly;
    int processOnlyCount;
    
#ifdef Q_WS_MAC
    SpeechChannel speechChannel;
#endif
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
    
#ifdef Q_WS_MAC
    OSErr theErr = noErr;
    theErr = NewSpeechChannel(NULL, &d->speechChannel);
    if (theErr != noErr) {
        qDebug() << "NewSpeechChannel() failed," << theErr;
        return;
    }
#endif
}

/*!
 * Deletes it.
 */
ExaminateTaskController::~ExaminateTaskController()
{
#ifdef Q_WS_MAC
    if (!d->speechChannel) return;
    
    OSErr theErr = noErr;
    theErr = StopSpeech(d->speechChannel);
    if (theErr != noErr)
        qDebug() << "StopSpeech() failed," << theErr;
    
    theErr = DisposeSpeechChannel(d->speechChannel);
    if (theErr != noErr)
        qDebug() << "DisposeSpeechChannel() failed," << theErr;
#endif
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

    //calculate timeline
    bool basedOnTyping = d->task->property("exam_basedOnTyping").toBool();
    bool limitExamTime = d->task->property("exam_limitExamTime").toBool();
    bool timeForQuestion = d->task->property("exam_timeForQuestion").toBool();

    int basedOnTypingValue = d->task->property("exam_basedOnTypingValue").toInt();
    int limitExamTimeValue = d->task->property("exam_limitExamTimeValue").toInt();
    int timeForQuestionValue = d->task->property("exam_timeForQuestionValue").toInt();

    int basedOnTypingUnit = d->task->property("exam_basedOnTypingUnit").toInt();
    int limitExamTimeUnit = d->task->property("exam_limitExamTimeUnit").toInt();
    int timeForQuestionUnit = d->task->property("exam_timeForQuestionUnit").toInt();

    basedOnTypingValue *= basedOnTypingUnit == CB_TIME_MINUTES ? 60000 : 1000;
    limitExamTimeValue *= limitExamTimeUnit == CB_TIME_MINUTES ? 60000 : 1000;
    timeForQuestionValue *= timeForQuestionUnit == CB_TIME_MINUTES ? 60000 : 1000;

    if ( !basedOnTyping && !limitExamTime && !timeForQuestion ) {
        basedOnTyping = true;
    }

    if (basedOnTyping) {
        int typingSpeed = app::typingSpeed(); //symbols in minute
        if ( typingSpeed <= 0 ) typingSpeed = 60;

        int typingTime = entry.answer.length() *1000 *60 /typingSpeed;

        entry.startTime = 0;
        entry.totalTime = basedOnTypingValue + typingTime;
    }
    else if (limitExamTime) {
        entry.startTime = d->usedTime;
        entry.totalTime = limitExamTimeValue;
    }
    else if (timeForQuestion) {
        entry.startTime = 0;
        entry.totalTime = timeForQuestionValue;
    }

    d->currentEntry = entry;
    return entry;
}

void ExaminateTaskController::processAnswer(int usedTime, QString answer)
{
    d->usedTime = usedTime;

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

    QString text = d->currentEntry.answer;

#ifdef Q_WS_MAC
    SpeakText(d->speechChannel , text.toLatin1().data(), text.toLatin1().size() );
#endif

    eventTimeMap.clear();
    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void ExaminateTaskController::readyForNext()
{
    emit requestNextQuestion();
}

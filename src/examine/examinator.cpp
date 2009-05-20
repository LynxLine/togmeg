//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "examinator.h"

#include "studytask.h"
#include "studytaskmodel.h"

#include "taskcontroller.h"
#include "playtaskcontroller.h"
#include "studytaskcontroller.h"

class Examinator::Private {
public:
    StudyTask * task;
    QTimeLine * timeLine;

    Mode mode;
    State state;
    QString userAnswer;
    QPointer<TaskController> controller;
};

/*!
 * Creates the object.
 */
Examinator::Examinator(QObject * parent)
:QObject(parent)
{
    d = new Private;
   
    d->task = 0L;
    d->timeLine = new QTimeLine(10000, this);
    d->timeLine->setCurveShape(QTimeLine::LinearCurve);
    d->timeLine->setFrameRange(0,100);
    d->timeLine->setLoopCount(1);

    setState(Stopped);

    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(timeLineFinished()), Qt::QueuedConnection);
    connect(d->timeLine, SIGNAL(frameChanged(int)),
            this, SIGNAL(tick(int)));
}

/*!
 * Deletes it.
 */
Examinator::~Examinator()
{
    qDebug() << "~Examinator()";
    delete d;
}

void Examinator::start(Examinator::Mode mode)
{
    Q_ASSERT( d->task );

    StudyTaskModel * model = StudyTaskModel::instance();

    d->mode = mode;

    if ( mode == Playing ) d->controller = new PlayTaskController( model );
    else if ( mode == Studying ) d->controller = new StudyTaskController( model );

    connect(d->controller, SIGNAL(requestNextQuestion()),
            this, SLOT(prepareNextQuestion()));
    connect(d->controller, SIGNAL(indicateMismatching()),
            this, SLOT(indicateMismatching()));
    connect(d->controller, SIGNAL(indicateMatching()),
            this, SLOT(indicateMatching()));
    connect(this, SIGNAL(userEvent(int, int)),
            d->controller, SLOT(collectUserEvent(int, int)));

    //update widget
    emit taskNameChanged( d->task->name() );
    emit modeChanged( mode );

    setState(Processing);
    prepareNextQuestion();
}

void Examinator::stop()
{
    Q_ASSERT( d->task );

    setState(Stopped);
    delete d->controller;

    d->timeLine->stop();
    d->timeLine->setCurrentTime(0);

    emit stopped();
}

void Examinator::continuePlay()
{
    Q_ASSERT( d->task );

    setState(Processing);
    d->timeLine->start();
}

void Examinator::pause()
{
    Q_ASSERT( d->task );

    setState(Paused);
    d->timeLine->stop();
}

QString Examinator::currentTaskId()
{
    if ( !d->task )
        return QString::null;

    return d->task->id();
}

QString Examinator::currentTaskName()
{
    if ( !d->task )
        return QString::null;

    return d->task->name();
}

int Examinator::entryCount()
{
    if ( !d->task ) return 0;
    return d->task->entryCount();
}

void Examinator::setEntryCount(int count)
{
    emit examinatorEnabled( count >0 );
}

void Examinator::prepareNextQuestion()
{
    if ( !d->controller->hasNext() ) {
        emit stopped();
        return;
    }

    ControllerDataEntry entry = d->controller->next();
    emit currentQuestionChanged( entry.question );
    emit currentAnswerChanged( entry.answer );

    setState(Processing);
    d->userAnswer.clear();
    d->timeLine->setDuration( entry.totalTime );
    d->timeLine->setCurrentTime( entry.startTime );
    d->timeLine->start();
}

void Examinator::setUserAnswer(QString answer)
{
    d->userAnswer = answer;
}

void Examinator::timeLineFinished()
{
    processAnswer(d->timeLine->duration());
}

void Examinator::processAnswerEarly()
{
    processAnswer(d->timeLine->currentTime());
}

void Examinator::processAnswer( int usedTime )
{
    if (d->task->property("exam_limitExamTime").toBool()) {
        //jump prohress to 100% to indicate finish
        d->timeLine->setCurrentTime( d->timeLine->duration() );
    }
    d->timeLine->stop();

    d->controller->processAnswer( usedTime, d->userAnswer );
}

Examinator::State Examinator::state()
{
    return d->state;
}

void Examinator::setState(Examinator::State s)
{
    if ( d->state != s )
        emit stateChanged(s);

    d->state = s;
}

void Examinator::indicateMismatching()
{
    setState(IndicatingMismatch);
}

void Examinator::indicateMatching()
{
    setState(IndicatingMatch);
}

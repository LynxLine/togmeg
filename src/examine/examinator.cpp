//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "examinator.h"

#include "studytask.h"
#include "tasklistmodel.h"
#include "studytaskmodel.h"

#include "taskcontroller.h"
#include "playtaskcontroller.h"
#include "studytaskcontroller.h"
#include "examinatetaskcontroller.h"

class Examinator::Private {
public:
    StudyTask * task;
    QTimeLine * timeLine;

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
            this, SLOT(processAnswer()), Qt::QueuedConnection);
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

    if ( mode == Playing ) d->controller = new PlayTaskController( model );
    else if ( mode == Studying ) d->controller = new StudyTaskController( model );
    else if ( mode == Examinating ) d->controller = new ExaminateTaskController( model );

    connect(d->controller, SIGNAL(requestNextQuestion()),
            this, SLOT(prepareNextQuestion()));
    connect(d->controller, SIGNAL(indicateMismatching()),
            this, SLOT(indicateMismatching()));
    connect(d->controller, SIGNAL(indicateMatching()),
            this, SLOT(indicateMatching()));

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

int Examinator::entryCount()
{
    if ( !d->task ) return 0;
    return d->task->entryCount();
}

void Examinator::setEntryCount(int count)
{
    emit examinatorEnabled( count >0 );
}

void Examinator::setCurrentTask(QString taskId)
{
    qDebug() << "Examinator::setCurrentTask()" << taskId;

    if ( taskId.isEmpty() ) {
        d->task = 0L;
        return;
    }

    if (d->task) {
        disconnect(d->task, SIGNAL(entryCountChanged(int)),
                   this,      SLOT(setEntryCount(int)));
    }

    d->task = TaskListModel::instance()->task( taskId );
    if ( !d->task ) return;

    connect(d->task, SIGNAL(entryCountChanged(int)),
            this,      SLOT(setEntryCount(int)));
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
    d->timeLine->setDuration( entry.msecs );
    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void Examinator::setUserAnswer(QString answer)
{
    d->userAnswer = answer;
}

void Examinator::processAnswer()
{
    d->timeLine->setCurrentTime( d->timeLine->duration() );
    d->timeLine->stop();

    d->controller->processAnswer( d->userAnswer );
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

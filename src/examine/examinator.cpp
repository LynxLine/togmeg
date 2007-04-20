//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "examinator.h"

#include "studytask.h"
#include "tasklistmodel.h"

class Examinator::Private {
public:
    StudyTask * task;
    QTimeLine * timeLine;

    State state;
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
            this, SLOT(prepareNextQuestion()), Qt::QueuedConnection);
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

void Examinator::start()
{
    Q_ASSERT( d->task );

    setState(Processing);
    prepareNextQuestion();

    //update widget
    emit taskNameChanged( d->task->name() );
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

void Examinator::stop()
{
    Q_ASSERT( d->task );

    setState(Stopped);
    d->timeLine->stop();
    d->timeLine->setCurrentTime(0);
}

void Examinator::setCurrentTask(QString taskId)
{
    qDebug() << "Examinator::setCurrentTask()" << taskId;

    if ( taskId.isEmpty() ) {
        d->task = 0L;
        return;
    }

    d->task = TaskListModel::instance()->task( taskId );
    if ( !d->task ) return;
}

void Examinator::prepareNextQuestion()
{
    d->timeLine->setDuration(10000);
    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
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

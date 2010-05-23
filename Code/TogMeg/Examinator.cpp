//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "examinator.h"

#include "TogMegFileModel.h"

#include "TaskController.h"
#include "PlayTaskController.h"
#include "StudyTaskController.h"

class Examinator::Private {
public:
    QTimeLine * timeLine;

    Mode mode;
    State state;
    QString userAnswer;
    
    QPointer<TogMegFileModel> model;
    QPointer<TaskController> controller;
};

/*!
 * Creates the object.
 */
Examinator::Examinator(TogMegFileModel * model)
:QObject(model)
{
    d = new Private;
    d->model = model;

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
    d->mode = mode;

    if ( mode == Playing ) d->controller = new PlayTaskController( d->model );
    else if ( mode == Studying ) d->controller = new StudyTaskController( d->model );

    connect(d->controller, SIGNAL(requestNextQuestion()),
            this, SLOT(prepareNextQuestion()));
    connect(d->controller, SIGNAL(indicateMismatching()),
            this, SLOT(indicateMismatching()));
    connect(d->controller, SIGNAL(indicateMatching()),
            this, SLOT(indicateMatching()));
    connect(this, SIGNAL(userEvent(int, int)),
            d->controller, SLOT(collectUserEvent(int, int)));

    //update widget
    emit modeChanged( mode );

    setState(Processing);
    prepareNextQuestion();
}

void Examinator::stop()
{
    setState(Stopped);
    delete d->controller;

    d->timeLine->stop();
    d->timeLine->setCurrentTime(0);

    emit stopped();
}

void Examinator::continuePlay()
{
    setState(Processing);
    d->timeLine->start();
}

void Examinator::pause()
{
    setState(Paused);
    d->timeLine->stop();
}

int Examinator::entryCount()
{
    return d->model->rowCount();
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

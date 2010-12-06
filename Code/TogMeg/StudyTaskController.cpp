
#include <QtGui>
#include "SpeechChannelBase.h"

#include "TogMeg.h"
#include "TogMegFileModel.h"
#include "StudyTaskController.h"

class StudyTaskController::Private {
public:
    Private():index(0),isLastFailed(false) {;}

    int index;
    bool isLastFailed;
    QTimeLine * timeLine;
    ControllerDataEntry currentEntry;
};

/*!
 * Creates the object.
 */
StudyTaskController::StudyTaskController(TogMegFileModel * parent)
:TaskController(parent)
{
    d = new Private;

    d->timeLine = new QTimeLine(3000, this);
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
    int nextIndex = 1;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, TogMegFileModel::ColQ);
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
        if (!d->isLastFailed)
            d->index = rand() % (model->rowCount()-1);
        QModelIndex i = model->index(d->index+1, TogMegFileModel::ColQ);
        question = model->data(i).toString();
    }

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, TogMegFileModel::ColA) ).toString();
    entry.question = model->data( model->index(d->index, TogMegFileModel::ColQ) ).toString();

    int typingSpeed = app::typingSpeed(); //symbols in minute
    if ( typingSpeed <= 0 ) typingSpeed = 60;

    int typingTime = entry.answer.length() *1000 *60 /typingSpeed;

    entry.startTime = 0;
    entry.totalTime = 5000 + typingTime;

    QModelIndex mi = model->index(d->index, TogMegFileModel::ColQ);
    model->setData(mi, true, TogMegFileModel::SpeechRole);

    d->currentEntry = entry;
 
    return entry;
}

void StudyTaskController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(usedTime);
    
    if ( d->timeLine->state() != QTimeLine::NotRunning ) {
        d->timeLine->setCurrentTime( d->timeLine->duration() );
        d->timeLine->stop();
        readyForNext();
        return;
    }

    if ( d->currentEntry.answer != answer.toLower().simplified() ) {
        d->isLastFailed = !d->isLastFailed;

        emit indicateMismatching();
        d->timeLine->setDuration( 1000 );
    }
    else {
        d->isLastFailed = false;

        emit indicateMatching();
        d->timeLine->setDuration( 300 );

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

    QModelIndex mi = model->index(d->index, TogMegFileModel::ColA);
    model->setData(mi, true, TogMegFileModel::SpeechRole);

    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void StudyTaskController::readyForNext()
{
    emit requestNextQuestion();
}

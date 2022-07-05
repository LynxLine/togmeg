
#include <QtGui>

#include "QAModel.h"
#include "PlayController.h"

class PlayController::Private {
public:
    Private():index(1) {;}

    int index;
};

/*!
 * Creates the object.
 */
PlayController::PlayController(QAModel * parent)
:TaskController(parent)
{
    d = new Private;
/*
    d->timeLine = new QTimeLine(3000, this);
    d->timeLine->setCurveShape(QTimeLine::LinearCurve);
    d->timeLine->setFrameRange(0,100);
    d->timeLine->setLoopCount(1);

    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(readyForNext()), Qt::QueuedConnection);*/
}

/*!
 * Deletes it.
 */
PlayController::~PlayController()
{
    delete d;
}

bool PlayController::hasNext()
{
    int nextIndex = d->index;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, QAModel::ColQ);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry PlayController::next()
{
    while ( d->index < model->rowCount() ) {
        QModelIndex i = model->index(d->index, QAModel::ColQ);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        d->index++;
    }

    ControllerDataEntry entry;

    entry.a = model->index(d->index, QAModel::ColA).data().toString();
    entry.q = model->index(d->index, QAModel::ColQ).data().toString();

    entry.totalTime = 5000; //temp
    entry.startTime = 0; //temp

    QModelIndex mi = model->index(d->index, QAModel::ColA);
    model->setData(mi, true, QAModel::SpeechRole);
    d->index++;

    return entry;
}

void PlayController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(usedTime);
    Q_UNUSED(answer);
    emit requestNextQuestion();
}

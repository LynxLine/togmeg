
#include <QtGui>
#include "SpeechChannelBase.h"

#include "TogMegFileModel.h"
#include "PlayTaskController.h"

class PlayTaskController::Private {
public:
    Private():index(0) {;}

    int index;
    QPointer<SpeechChannelBase> speech;
};

/*!
 * Creates the object.
 */
PlayTaskController::PlayTaskController(TogMegFileModel * parent)
:TaskController(parent)
{
    d = new Private;
    d->speech = SpeechChannelBase::Create(this);
}

/*!
 * Deletes it.
 */
PlayTaskController::~PlayTaskController()
{
    delete d;
}

bool PlayTaskController::hasNext()
{
    int nextIndex = d->index;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, TogMegFileModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry PlayTaskController::next()
{
    while ( d->index < model->rowCount() ) {
        QModelIndex i = model->index(d->index, TogMegFileModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        d->index++;
    }

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, TogMegFileModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index, TogMegFileModel::QuestionColumn) ).toString();

    entry.totalTime = 5000; //temp
    entry.startTime = 0; //temp

    QString text = entry.answer;

    d->speech->start(text);
    d->index++;

    return entry;
}

void PlayTaskController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(usedTime);
    Q_UNUSED(answer);
    emit requestNextQuestion();
}

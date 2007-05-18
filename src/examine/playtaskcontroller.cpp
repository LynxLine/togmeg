//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytaskmodel.h"
#include "playtaskcontroller.h"

class PlayTaskController::Private {
public:
    Private():index(0) {;}

    int index;
};

/*!
 * Creates the object.
 */
PlayTaskController::PlayTaskController(StudyTaskModel * parent)
:TaskController(parent)
{
    d = new Private;
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
        QModelIndex i = model->index(nextIndex, StudyTaskModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry PlayTaskController::next()
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

    entry.totalTime = 5000; //temp
    entry.startTime = 0; //temp

    d->index++;

    return entry;
}

void PlayTaskController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(answer);
    emit requestNextQuestion();
}

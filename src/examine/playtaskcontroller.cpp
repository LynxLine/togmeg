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
    return d->index < model->rowCount();
}

ControllerDataEntry PlayTaskController::next()
{
    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index, StudyTaskModel::QuestionColumn) ).toString();
    entry.msecs = 5000; //temp

    d->index++;

    return entry;
}

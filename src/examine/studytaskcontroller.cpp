//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytaskmodel.h"
#include "studytaskcontroller.h"

class StudyTaskController::Private {
public:
    Private():index(0) {;}

    int index;
};

/*!
 * Creates the object.
 */
StudyTaskController::StudyTaskController(StudyTaskModel * parent)
:TaskController(parent)
{
    d = new Private;
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
    return d->index < model->rowCount();
}

ControllerDataEntry StudyTaskController::next()
{
    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index++, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index++, StudyTaskModel::QuestionColumn) ).toString();
    entry.msecs = 5000; //temp

    d->index++;

    return entry;
}

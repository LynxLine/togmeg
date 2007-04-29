//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytaskmodel.h"
#include "examinatetaskcontroller.h"

class ExaminateTaskController::Private {
public:
    Private():index(0) {;}

    int index;
};

/*!
 * Creates the object.
 */
ExaminateTaskController::ExaminateTaskController(StudyTaskModel * parent)
:TaskController(parent)
{
    d = new Private;
}

/*!
 * Deletes it.
 */
ExaminateTaskController::~ExaminateTaskController()
{
    delete d;
}

bool ExaminateTaskController::hasNext()
{
    return d->index < model->rowCount();
}

ControllerDataEntry ExaminateTaskController::next()
{
    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index++, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index++, StudyTaskModel::QuestionColumn) ).toString();
    entry.msecs = 5000; //temp

    d->index++;

    return entry;
}

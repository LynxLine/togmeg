//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "taskcontroller.h"
#include "studytaskmodel.h"

/*!
 * Creates the object.
 */
TaskController::TaskController(StudyTaskModel * parent)
:QObject(parent), model(parent)
{
}

/*!
 * Deletes it.
 */
TaskController::~TaskController()
{
}

bool ControllerDataEntry::isNull() {
    return question.isEmpty() && answer.isEmpty() && msecs == 0;
}

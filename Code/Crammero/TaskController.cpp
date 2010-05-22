//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "taskcontroller.h"
#include "CramFileModel.h"

/*!
 * Creates the object.
 */
TaskController::TaskController(CramFileModel * parent)
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
    return question.isEmpty() && answer.isEmpty() && totalTime == 0;
}

void TaskController::collectUserEvent(int msecs, int eventType)
{
    eventTimeMap[ msecs ] = eventType;
}

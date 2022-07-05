
#include <QtGui>
#include "TaskController.h"
#include "QAModel.h"

/*!
 * Creates the object.
 */
TaskController::TaskController(QAModel * parent)
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
    return q.isEmpty() && a.isEmpty() && totalTime == 0;
}

void TaskController::collectUserEvent(int msecs, int eventType)
{
    eventTimeMap[ msecs ] = eventType;
}

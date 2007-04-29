//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytaskcontroller.h"

class StudyTaskController::Private {
public:
};

/*!
 * Creates the object.
 */
StudyTaskController::StudyTaskController(QObject * parent)
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

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "examinatetaskcontroller.h"

class ExaminateTaskController::Private {
public:
};

/*!
 * Creates the object.
 */
ExaminateTaskController::ExaminateTaskController(QObject * parent)
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

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "playtaskcontroller.h"

class PlayTaskController::Private {
public:
};

/*!
 * Creates the object.
 */
PlayTaskController::PlayTaskController(QObject * parent)
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

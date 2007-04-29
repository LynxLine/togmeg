//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMINATETASKCONTROLLER_H
#define EXAMINATETASKCONTROLLER_H

#include "taskcontroller.h"

/*!
 * \class ExaminateTaskController
 */
class ExaminateTaskController : public TaskController
{
Q_OBJECT
public:
    ExaminateTaskController(QObject * parent = 0);
    virtual ~ExaminateTaskController();

private:
    class Private;
    Private * d;
};

#endif // EXAMINATETASKCONTROLLER_H

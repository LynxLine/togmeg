//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QObject>

/*!
 * \class TaskController
 */
class TaskController : public QObject
{
Q_OBJECT
public:
    TaskController(QObject * parent = 0);
    virtual ~TaskController();

};

#endif // TASKCONTROLLER_H

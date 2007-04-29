//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef STUDYTASKCONTROLLER_H
#define STUDYTASKCONTROLLER_H

#include "taskcontroller.h"

/*!
 * \class StudyTaskController
 */
class StudyTaskController : public TaskController
{
Q_OBJECT
public:
    StudyTaskController(QObject * parent = 0);
    virtual ~StudyTaskController();

private:
    class Private;
    Private * d;
};

#endif // STUDYTASKCONTROLLER_H

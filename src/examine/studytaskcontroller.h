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
    StudyTaskController(StudyTaskModel * parent = 0);
    virtual ~StudyTaskController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

private:
    class Private;
    Private * d;
};

#endif // STUDYTASKCONTROLLER_H

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef PLAYTASKCONTROLLER_H
#define PLAYTASKCONTROLLER_H

#include "taskcontroller.h"

/*!
 * \class PlayTaskController
 */
class PlayTaskController : public TaskController
{
Q_OBJECT
public:
    PlayTaskController(CramFileModel * parent = 0);
    virtual ~PlayTaskController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

    virtual void processAnswer(int usedTime, QString);

private:
    class Private;
    Private * d;
};

#endif // PLAYTASKCONTROLLER_H

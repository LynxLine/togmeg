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
    ExaminateTaskController(StudyTaskModel * parent = 0);
    virtual ~ExaminateTaskController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

    virtual void processAnswer(int usedTime, QString);

private slots:
    void readyForNext();

private:
    class Private;
    Private * d;
};

#endif // EXAMINATETASKCONTROLLER_H

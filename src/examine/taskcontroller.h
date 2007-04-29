//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QObject>
class StudyTaskModel;

class ControllerDataEntry {
public:
    ControllerDataEntry():msecs(0) {;}
    bool isNull();

    QString question;
    QString answer;
    int msecs;
};

/*!
 * \class TaskController
 */
class TaskController : public QObject
{
Q_OBJECT
public:
    TaskController(StudyTaskModel * parent);
    virtual ~TaskController();

    virtual bool hasNext() = 0;
    virtual ControllerDataEntry next() = 0;

protected:
    StudyTaskModel * model;
};

#endif // TASKCONTROLLER_H

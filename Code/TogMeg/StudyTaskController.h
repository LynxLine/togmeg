
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
    StudyTaskController(TogMegFileModel * parent = 0);
    virtual ~StudyTaskController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

    virtual void processAnswer(int usedTime, QString);

private slots:
    void readyForNext();

private:
    class Private;
    Private * d;
};

#endif // STUDYTASKCONTROLLER_H

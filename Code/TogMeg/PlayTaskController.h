
#ifndef PLAYTASKCONTROLLER_H
#define PLAYTASKCONTROLLER_H

#include "TaskController.h"

/*!
 * \class PlayTaskController
 */
class PlayTaskController : public TaskController
{
Q_OBJECT
public:
    PlayTaskController(TogMegFileModel * parent = 0);
    virtual ~PlayTaskController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

    virtual void processAnswer(int usedTime, QString);

private:
    class Private;
    Private * d;
};

#endif // PLAYTASKCONTROLLER_H

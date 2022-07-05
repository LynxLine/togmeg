
#ifndef PlayController_H
#define PlayController_H

#include "TaskController.h"

/*!
 * \class PlayController
 */
class PlayController : public TaskController
{
Q_OBJECT
public:
    PlayController(QAModel * parent = 0);
    virtual ~PlayController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

    virtual void processAnswer(int usedTime, QString);

private:
    class Private;
    Private * d;
};

#endif // PlayController_H

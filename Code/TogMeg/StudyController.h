
#ifndef StudyController_H
#define StudyController_H

#include "TaskController.h"

/*!
 * \class StudyController
 */
class StudyController : public TaskController
{
Q_OBJECT
public:
    StudyController(QAModel * parent = 0);
    virtual ~StudyController();

    virtual bool hasNext();
    virtual ControllerDataEntry next();

    virtual void processAnswer(int usedTime, QString);

private slots:
    void readyForNext();

private:
    class Private;
    Private * d;
};

#endif // StudyController_H

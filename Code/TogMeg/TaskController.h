
#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QMap>
#include <QObject>
class TogMegFileModel;

class ControllerDataEntry {
public:
    ControllerDataEntry():startTime(0),totalTime(0) {;}
    bool isNull();

    QString q;
    QString a;

    int startTime; //msecs
    int totalTime; //msecs
};

/*!
 * \class TaskController
 */
class TaskController : public QObject
{
Q_OBJECT
public:
    TaskController(TogMegFileModel * parent);
    virtual ~TaskController();

    virtual bool hasNext() = 0;
    virtual ControllerDataEntry next() = 0;

    virtual void processAnswer(int usedTime, QString answer) {
        Q_UNUSED(usedTime);
        Q_UNUSED(answer);
        eventTimeMap.clear();
    }

public slots:
    virtual void collectUserEvent(int msecs, int eventType);

signals:
    void requestNextQuestion();
    void indicateMismatching();
    void indicateMatching();

protected:
    TogMegFileModel * model;
    QMap<int, int> eventTimeMap;
};

#endif // TASKCONTROLLER_H

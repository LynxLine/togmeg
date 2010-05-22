//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QMap>
#include <QObject>
class CramFileModel;

class ControllerDataEntry {
public:
    ControllerDataEntry():startTime(0),totalTime(0) {;}
    bool isNull();

    QString question;
    QString answer;
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
    TaskController(CramFileModel * parent);
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
    CramFileModel * model;
    QMap<int, int> eventTimeMap;
};

#endif // TASKCONTROLLER_H

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMINATOR_H
#define EXAMINATOR_H

#include <QObject>

/*!
 * \class Examinator
 */
class Examinator : public QObject
{
Q_OBJECT
public:
    enum State {
        Processing = 0,
        Stopped,
        Paused
    };

    Examinator(QObject * parent = 0);
    virtual ~Examinator();

    State state();

public slots:
    void start();
    void pause();
    void stop();
    void continuePlay();

    void setCurrentTask(QString taskId);

signals:
    void tick(int percent);
    void taskNameChanged(QString name);
    void currentQuestionChanged(QString question);
    void stateChanged(Examinator::State);

private slots:
    void setState(State);
    void prepareNextQuestion();

private:
    class Private;
    Private * d;
};

#endif // EXAMINATOR_H

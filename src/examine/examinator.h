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
    enum Mode {
        Playing = 0,
        Studying,
        Examinating,
    };
    enum State {
        Processing = 0,
        Stopped,
        Paused,
    };

    Examinator(QObject * parent = 0);
    virtual ~Examinator();

    State state();
    QString currentTaskId();
    int entryCount();

public slots:
    void start(Examinator::Mode);
    void pause();
    void stop();
    void continuePlay();

    void setCurrentTask(QString taskId);

signals:
    void stopped();
    void tick(int percent);
    void examinatorEnabled(bool);
    void taskNameChanged(QString);
    void currentAnswerChanged(QString);
    void currentQuestionChanged(QString);

    void stateChanged(Examinator::State);
    void modeChanged(Examinator::Mode);

private slots:
    void setState(State);
    void setEntryCount(int);
    void prepareNextQuestion();

private:
    class Private;
    Private * d;
};

#endif // EXAMINATOR_H

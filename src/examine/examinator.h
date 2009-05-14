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
        IndicatingMatch,
        IndicatingMismatch,
    };

    Examinator(QObject * parent = 0);
    virtual ~Examinator();

    State state();
    QString currentTaskId();
    QString currentTaskName();
    int entryCount();

public slots:
    void start(Examinator::Mode);
    void pause();
    void stop();
    void continuePlay();

    void processAnswerEarly();
    void processAnswer(int usedTime);

    void setUserAnswer(QString);

signals:
    void stopped();
    void tick(int percent);
    void examinatorEnabled(bool);
    void taskNameChanged(QString);
    void currentAnswerChanged(QString);
    void currentQuestionChanged(QString);
    void userEvent(int msecs, int eventType);

    void stateChanged(Examinator::State);
    void modeChanged(Examinator::Mode);

private slots:
    void setState(State);
    void setEntryCount(int);
    void timeLineFinished();
    void prepareNextQuestion();
    void indicateMismatching();
    void indicateMatching();

private:
    class Private;
    Private * d;
};

#endif // EXAMINATOR_H

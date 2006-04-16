// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef STUDYPROCESSOR_H
#define STUDYPROCESSOR_H

#include <QObject>

class MainWindow;
class StudyStorage;

/*!
 * \class StudyProcessor
 */
class StudyProcessor : public QObject
{
Q_OBJECT
public:
    StudyProcessor(MainWindow * parent = 0);
    ~StudyProcessor();

    StudyStorage * storage();
    
public slots:
    void start();
    void stop();

private slots:
    void startAsking();
    void startProgress();
    void waitingForAnswer();
    void returnPressed();
    void checkAnswer();
    
private:
    class Private;
    Private * d;
};

#endif // STUDYPROCESSOR_H


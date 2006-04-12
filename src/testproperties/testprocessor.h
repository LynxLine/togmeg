// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#ifndef TESTPROCESSOR_H
#define TESTPROCESSOR_H

#include <QObject>

class MainWindow;

/*!
 * \class TestProcessor
 */
class TestProcessor : public QObject
{
Q_OBJECT
public:
    TestProcessor(MainWindow * parent = 0);
    ~TestProcessor();
    
public slots:
    void start();
    void stop();

private:
    class Private;
    Private * d;
};

#endif // TESTPROCESSOR_H


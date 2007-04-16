//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef STUDYTASK_H
#define STUDYTASK_H

#include <QObject>
class DataContainer;

/*!
 * \class StudyTask
 */
class StudyTask : public QObject
{
friend class StudyTaskModel;
Q_OBJECT
public:
    StudyTask(QObject * parent = 0);
    StudyTask(DataContainer * container, QObject * parent = 0);
    virtual ~StudyTask();

    QString id();
    QString name();
    QString categoryId();

public slots:
    void setName(QString);
    void setCategoryId(QString);

private:
    DataContainer * dataContainer();

private:
    class Private;
    Private * d;
};

#endif // STUDYTASK_H

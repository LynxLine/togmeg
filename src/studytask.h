//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef STUDYTASK_H
#define STUDYTASK_H

#include <QObject>
#include <QVariant>
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
    QVariant property(QString);
    int entryCount();

public slots:
    void setName(QString);
    void setCategoryId(QString);
    void setProperty(QString name, QVariant value);
    void setEntryCount(int);

signals:
    void entryCountChanged(int);

private:
    class Private;
    Private * d;
};

#endif // STUDYTASK_H

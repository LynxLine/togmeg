//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include <QAbstractListModel>

class StudyTask;

class TaskListModel : public QAbstractListModel
{
Q_OBJECT

public:
    TaskListModel(QObject * parent = 0L);
    virtual ~TaskListModel();

    QList<StudyTask *> tasks();
    StudyTask * task(QModelIndex) const;
    QModelIndex indexOf(StudyTask *) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public slots:
    void addTask(StudyTask *);

private:
	class Private;
	Private * d;

};

#endif // TASKLISTMODEL_H

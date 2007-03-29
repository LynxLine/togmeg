//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include <QAbstractListModel>

class TaskListModel : public QAbstractListModel
{
Q_OBJECT

public:
    TaskListModel(QObject * parent = 0L);
    virtual ~TaskListModel();

private:
	class Private;
	Private * d;

};

#endif // TASKLISTMODEL_H

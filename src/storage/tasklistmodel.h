//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include <QAbstractItemModel>

class TaskListModel : public QAbstractItemModel
{
Q_OBJECT

public:
    TaskListModel(QObject * parent = 0L);
    virtual ~TaskListModel();

    QModelIndex parent(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

private:
	class Private;
	Private * d;

};

#endif // TASKLISTMODEL_H

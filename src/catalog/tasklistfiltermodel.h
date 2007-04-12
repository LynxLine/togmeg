//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKLISTFILTERMODEL_H
#define TASKLISTFILTERMODEL_H

#include <QSortFilterProxyModel>

/*!
 * \class TaskListFilterModel
 */
class TaskListFilterModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    TaskListFilterModel(QObject * parent = 0);
    ~TaskListFilterModel();

    bool isCategoryFilteringEnabled();
    QString categoryId();

    void setCategoryFiltering(QString id);
    void clearFiltering();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

private:
    class Private;
    Private * d;
};

#endif // TASKLISTFILTERMODEL_H

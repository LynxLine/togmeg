//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "tasklistmodel.h"
#include "tasklistfiltermodel.h"

class TaskListFilterModel::Private {
public:
    bool categoryFiltering;
    QString categoryId;
};

/*!
 * Creates the object.
 */
TaskListFilterModel::TaskListFilterModel(QObject * parent)
:QSortFilterProxyModel(parent)
{
    d = new Private;
    d->categoryFiltering = false;
    setSortRole( TaskListModel::SortRole );
}

/*!
 * Deletes it.
 */
TaskListFilterModel::~TaskListFilterModel()
{
    delete d;
}

bool TaskListFilterModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    bool f = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    if ( !d->categoryFiltering ) return f;
    if ( !index.isValid() ) return f;
    
    TaskListModel * taskListModel = (TaskListModel *)sourceModel();
    StudyTask * studyTask = taskListModel->task(index);
    if ( !studyTask ) return f;
    
    return f && studyTask->categoryId().startsWith( d->categoryId );
}

void TaskListFilterModel::setCategoryFiltering(QString id)
{
    if (id.isEmpty()) {
        clearFiltering();
        return;
    }

    d->categoryFiltering = true;
    d->categoryId = id;
    clear();
}

void TaskListFilterModel::clearFiltering()
{
    d->categoryFiltering = false;
    d->categoryId.clear();
    clear();
}

bool TaskListFilterModel::isCategoryFilteringEnabled()
{
    return d->categoryFiltering;
}

QString TaskListFilterModel::categoryId()
{
    if ( !d->categoryFiltering ) return QString::null;
    return d->categoryId;
}

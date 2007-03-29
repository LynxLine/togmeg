//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "tasklistmodel.h"

class TaskListModel::Private
{
public:
};

/*!
 Crestes TaskListModel
 */
TaskListModel::TaskListModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
}

/*!
 Deletes the object.
 */
TaskListModel::~TaskListModel()
{
    qDebug() << "~TaskListModel";
    delete d;
}

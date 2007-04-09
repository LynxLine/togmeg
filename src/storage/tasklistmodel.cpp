//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "crammero.h"
#include "datacontainer.h"
#include "tasklistmodel.h"

class TaskListModel::Private
{
public:
    QList<DataContainer *> containers;
};

/*!
 Crestes TaskListModel
 */
TaskListModel::TaskListModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;

    QDir storageDir(app::storagePath());
    QStringList dirs = storageDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString dirName, dirs) {
        QString taskPath = app::storagePath()+dirName;
        DataContainer * container = DataContainerFactory::resourceContainer( taskPath );
        if ( !container ) continue;
        d->containers << container;
    }
}

/*!
 Deletes the object.
 */
TaskListModel::~TaskListModel()
{
    qDebug() << "~TaskListModel";
    delete d;
}

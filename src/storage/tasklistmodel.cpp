//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "crammero.h"
#include "studytask.h"
#include "datacontainer.h"
#include "tasklistmodel.h"

class TaskListModel::Private
{
public:
    static TaskListModel * instance;
    QList<StudyTask *> tasks;
};

TaskListModel * TaskListModel::Private::instance = 0L;

/*!
 Crestes TaskListModel
 */
TaskListModel::TaskListModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
    Private::instance = this;

    QDir storageDir(app::storagePath());
    QStringList dirs = storageDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString dirName, dirs) {
        QString taskPath = app::storagePath()+dirName;
        DataContainer * container = DataContainerFactory::resourceContainer( taskPath );
        if ( !container ) continue;
        
        StudyTask * task = new StudyTask(container, this);
        d->tasks << task;
    }
}

TaskListModel * TaskListModel::instance()
{
    return Private::instance;
}

/*!
 Deletes the object.
 */
TaskListModel::~TaskListModel()
{
    qDebug() << "~TaskListModel";
    delete d;
}

QList<StudyTask *> TaskListModel::tasks()
{
    return d->tasks;
}

StudyTask * TaskListModel::task(QModelIndex index) const
{
    if ( !index.isValid() ) return 0L;
    if ( index.row() < 0 || index.row() >= d->tasks.count() ) return 0L;
    return d->tasks[ index.row() ];
}

StudyTask * TaskListModel::task(QString taskId) const
{
    StudyTask * task = 0L;
    for (int i=0;i<d->tasks.count();i++) {
        if ( d->tasks[i]->id() == taskId ) {
            task = d->tasks[i];
            break;
        }
    }

    return task;
}

QModelIndex TaskListModel::indexOf(StudyTask * task) const
{
    for (int i=0;i<d->tasks.count();i++) {
        if ( d->tasks[i] == task ) {
            QModelIndex index = createIndex(i,0, task);
            return index;
        }
    }

    return QModelIndex();
}

int TaskListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->tasks.count();
}

QVariant TaskListModel::data(const QModelIndex & index, int role) const
{
    StudyTask * studyTask = task(index);
    if ( !studyTask ) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return studyTask->name();
    }

    return QVariant();
}

bool TaskListModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    StudyTask * studyTask = task(index);
    if ( !studyTask ) return false;
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        studyTask->setName( value.toString() );
        return true;
    }

    return false;
}

QVariant TaskListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    if (role == Qt::DisplayRole) {
        return tr("Name");
    }
    return QVariant();
}

/*!
 \reimp for Category-like Model.
 */
Qt::ItemFlags TaskListModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= d->tasks.count() ) return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void TaskListModel::addTask(StudyTask * task)
{
    beginInsertRows(QModelIndex(), d->tasks.count(), d->tasks.count());
    d->tasks << task;
    endInsertRows();
}

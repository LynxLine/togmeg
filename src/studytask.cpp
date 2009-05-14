//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtXml>
#include "crammero.h"
#include "studytask.h"
#include "datacontainer.h"

class StudyTask::Private {
public:
    QMap<QString, QVariant> properties;
    DataContainer * dataContainer;
};

/*!
 * Creates the object.
 * Creates new container with unique id
 */
StudyTask::StudyTask(QObject * parent)
:QObject(parent)
{
    d = new Private;

    setProperty("id", app::uniqueId());
    setProperty("name", tr("New study"));
    setProperty("count", 0);

    QDir storageDir(app::storagePath());
    storageDir.mkpath( id() );

    QString taskPath = app::storagePath() +id();
    DataContainer * container = DataContainerFactory::resourceContainer( taskPath );

    d->dataContainer = container;
}

/*!
 * Creates the object and load datas from container.
 */
StudyTask::StudyTask(DataContainer * container, QObject * parent)
:QObject(parent)
{
    d = new Private;

    Q_ASSERT( container != 0L );
    d->dataContainer = container;
}

/*!
 * Deletes it.
 */
StudyTask::~StudyTask()
{
    delete d->dataContainer;
    delete d;
}

QString StudyTask::id()
{
    return property("id").toString();
}

QString StudyTask::name()
{
    return property("name").toString();
}

void StudyTask::setName(QString name)
{
    setProperty("name", name);
}

QString StudyTask::categoryId()
{
    return property("category").toString();
}

void StudyTask::setCategoryId(QString id)
{
    setProperty("category", id);
}

DataContainer * StudyTask::dataContainer()
{
    return d->dataContainer;
}

int StudyTask::entryCount()
{
    return property("count").toInt();
}

void StudyTask::setEntryCount(int count)
{
    if ( entryCount() != count)
        emit entryCountChanged(count);

    setProperty("count", count);
}

QVariant StudyTask::property(QString name)
{
    return d->properties[ name ];
}

void StudyTask::setProperty(QString name, QVariant value)
{
    d->properties[ name ] = value;
}

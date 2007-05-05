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
    QString id;
    QString name;
    QString categoryId;
    int entryCount;

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

    d->id = app::uniqueId();
    d->name = tr("New study");
    d->entryCount = 0;

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

    QIODevice * resource = d->dataContainer->resource("info.xml");
    if (resource) {
        QDomDocument doc;
        doc.setContent( resource );
        delete resource;

        QDomElement el = doc.documentElement();
        d->entryCount = el.attribute("count").toInt();
        d->categoryId = el.attribute("category");
        d->name = el.attribute("name");
        d->id = el.attribute("id");
    }
}

/*!
 * Deletes it.
 */
StudyTask::~StudyTask()
{
    //qDebug() << "~StudyTask()";
    QIODevice * resource = d->dataContainer->create("info.xml");
    if (resource) {
        QDomDocument doc("taskinfoxml");

        QDomElement child = doc.createElement("studytask");
        child.setAttribute("count", entryCount());
        child.setAttribute("category", categoryId());
        child.setAttribute("name", name());
        child.setAttribute("id", id());
        doc.appendChild(child);

        resource->write(doc.toByteArray());
        delete resource;
    }

    delete d->dataContainer;
    delete d;
}

QString StudyTask::id()
{
    return d->id;
}

QString StudyTask::name()
{
    return d->name;
}

void StudyTask::setName(QString name)
{
    d->name = name;
}

QString StudyTask::categoryId()
{
    return d->categoryId;
}

void StudyTask::setCategoryId(QString id)
{
    d->categoryId = id;
}

DataContainer * StudyTask::dataContainer()
{
    return d->dataContainer;
}

int StudyTask::entryCount()
{
    return d->entryCount;
}

void StudyTask::setEntryCount(int count)
{
    if (d->entryCount != count)
        emit entryCountChanged(count);
    d->entryCount = count;
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "crammero.h"
#include "studytask.h"
#include "datacontainer.h"

class StudyTask::Private {
public:
    QString id;
    QString name;
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
        d->name = el.attribute("name");
        d->id = el.attribute("id");
    }
}

/*!
 * Deletes it.
 */
StudyTask::~StudyTask()
{
    QIODevice * resource = d->dataContainer->create("info.xml");
    if (resource) {
        QDomDocument doc("taskinfoxml");

        QDomElement child = doc.createElement("studytask");
        child.setAttribute("name", name());
        child.setAttribute("id", id());
        doc.appendChild(child);

        resource->write(doc.toByteArray());
        delete resource;
    }

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

DataContainer * StudyTask::dataContainer()
{
    return d->dataContainer;
}

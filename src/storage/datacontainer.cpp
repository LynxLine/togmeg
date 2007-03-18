//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "datacontainer.h"
#include "dircontainer.h"

class DataContainer::Private {
public:
    QString location;
    QIODevice::OpenMode mode;
};

DataContainer::DataContainer(QString location, QIODevice::OpenMode mode)
{
    d = new Private;
    d->location = location;
    d->mode = mode;
}

DataContainer::~DataContainer()
{
	delete d;
}

bool DataContainer::isReadable()
{
    return d->mode & QIODevice::ReadOnly;
}

bool DataContainer::isWritable()
{
    return d->mode & QIODevice::WriteOnly;
}

QIODevice::OpenMode DataContainer::openMode()
{
    return d->mode;
}

QString DataContainer::location()
{
    return d->location;
}

DataContainer* DataContainerFactory::resourceContainer( QString location )
{
    if ( QFileInfo(location).isDir() ) {
        return new DirContainer( location, QIODevice::ReadOnly);
    }

    qDebug() << location << "can not be open";
    return 0;
}

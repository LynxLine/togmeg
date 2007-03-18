//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "dircontainer.h"

#define NO_ATTRIBUTES 0x0000
#define COMPRESSION   0x0001

class DirContainer::Private
{
public:
    QDir * container;
	QString containerPath;
};

DirContainer::DirContainer(QString containerPath, QIODevice::OpenMode mode) 
:DataContainer(containerPath, mode)
{
	d = new Private;
	d->containerPath = containerPath;
    d->container = new QDir( containerPath );
}

DirContainer::~DirContainer() 
{
    delete d->container;
	delete d;
}

bool DirContainer::exists()
{
	return d->container->exists();
}

QStringList DirContainer::resourceList(QStringList nameFilters)
{
	return d->container->entryList(nameFilters, QDir::Files);
}

bool DirContainer::contains(QString nameFilter)
{
    QStringList nameFilters( nameFilter );
    return !resourceList( nameFilters ).empty();
}
 
long DirContainer::resourceSize(QString resourceName)
{		
    QString filePath = location() +"/" + resourceName;
	QFileInfo resource( filePath );
	return resource.size();
}

QIODevice * DirContainer::resource(QString resourceName)
{
	QFile * resource = NULL;
	
	QString filePath = location() +"/" + resourceName;
	if ( !QFileInfo(filePath).exists() ) 
		return 0L;
		
	resource = new QFile( filePath );
	resource->open( openMode() );
    return resource;
}

QIODevice * DirContainer::create(QString resourceName)
{
	QString filePath = location() +"/" + resourceName;
	QFile * resource = new QFile( filePath );
	resource->open( openMode() & QIODevice::WriteOnly );
    if (!resource->exists()) {
        delete resource;
        return 0L;
    }
    return resource;
}

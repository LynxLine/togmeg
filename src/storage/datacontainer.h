//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QIODevice>

class DataContainer  
{
public:
    DataContainer(QString location, QIODevice::OpenMode);
    virtual ~DataContainer();
    QString location();
    
    bool isReadable();
    bool isWritable();
    QIODevice::OpenMode openMode();
    
    virtual bool exists() = 0;
    virtual bool contains(QString nameFilter) = 0;
	
    virtual QStringList resourceList(QStringList nameFilters) = 0;
    virtual QIODevice * resource(QString resourceName) = 0;
	virtual QIODevice * create(QString resourceName) = 0;
    virtual long resourceSize(QString resourceName) = 0;

private: 
    class Private;
    Private * d;
};

class DataContainerFactory
{
public:
    static DataContainer* resourceContainer( QString bookPath );
};

#endif // DATACONTAINER_H

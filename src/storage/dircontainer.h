//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef DIRCONTAINER
#define DIRCONTAINER

#include "datacontainer.h"

/*! 
 * Realization of the F4ResourceContainer interface.
 * For details on each virtual method see the parent class.
 */
class DirContainer : public DataContainer  
{
public:
    DirContainer(QString containerPath, QIODevice::OpenMode);
    virtual ~DirContainer();

    virtual bool exists();
    virtual bool contains(QString nameFilter);
    
    virtual QStringList resourceList(QStringList nameFilters);
    virtual QIODevice * resource(QString resourceName);
	virtual QIODevice * create(QString resourceName);
    virtual long resourceSize(QString resourceName);
 
private:
    class Private;
    Private * d;
};

#endif // DIRCONTAINER

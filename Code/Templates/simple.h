//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CLASS_H
#define CLASS_H

#include <QObject>

/*!
 * \class Class
 */
class Class : public QObject
{
Q_OBJECT
public:
    Class(QObject * parent = 0);
    virtual ~Class();

};

#endif // CLASS_H

// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

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
    ~Class();

};

#endif // CLASS_H


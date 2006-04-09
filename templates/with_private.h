// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef CLASS_H
#define CLASS_H

#include <QWidget>

/*!
 * \class Class
 */
class Class : public QWidget
{
Q_OBJECT
public:
    Class(QWidget * parent = 0);
    ~Class();

private:
    class Private;
    Private * d;
};

#endif // CLASS_H


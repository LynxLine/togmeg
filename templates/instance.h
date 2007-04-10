//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

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
    static Class * instance();
    virtual ~Class();

private:
    class Private;
    Private * d;
};

#endif // CLASS_H

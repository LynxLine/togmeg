// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "instance.h"

class Class::Private {
public:
    static Class * instance;
};

Class * Class::Private::instance = 0L;

/*!
 * Creates the object.
 */
Class::Class(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    Private::instance = this;
}

/*!
 * Deletes it.
 */
Class::~Class()
{
    delete d;
}

/*!
 * Returns the instance.
 */
Class * Class::instance()
{
    if ( !Class::Private::instance ) {
        Class::Private::instance = new Class;
    }
    return Class::Private::instance;
}


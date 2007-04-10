//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

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

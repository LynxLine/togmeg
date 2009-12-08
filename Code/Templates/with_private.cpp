//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "with_private.h"

class Class::Private {
public:
};

/*!
 * Creates the object.
 */
Class::Class(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
}

/*!
 * Deletes it.
 */
Class::~Class()
{
    delete d;
}

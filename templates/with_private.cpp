// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

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


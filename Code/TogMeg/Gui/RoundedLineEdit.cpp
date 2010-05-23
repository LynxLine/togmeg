//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "roundedlineedit.h"

class RoundedLineEdit::Private {
public:
};

/*!
 * Creates the object.
 */
RoundedLineEdit::RoundedLineEdit(QWidget * parent)
:QLineEdit(parent)
{
    d = new Private;
}

/*!
 * Deletes it.
 */
RoundedLineEdit::~RoundedLineEdit()
{
    delete d;
}

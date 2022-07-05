
#include <QtGui>
#include "RoundedLineEdit.h"

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


#ifndef ROUNDEDLINEEDIT_H
#define ROUNDEDLINEEDIT_H

#include <QLineEdit>

/*!
 * \class RoundedLineEdit
 */
class RoundedLineEdit : public QLineEdit
{
Q_OBJECT
public:
    RoundedLineEdit(QWidget * parent = 0);
    virtual ~RoundedLineEdit();

private:
    class Private;
    Private * d;
};

#endif // ROUNDEDLINEEDIT_H

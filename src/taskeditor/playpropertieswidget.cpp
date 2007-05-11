//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "playpropertieswidget.h"

class PlayPropertiesWidget::Private {
public:
};

/*!
 * Creates the object.
 */
PlayPropertiesWidget::PlayPropertiesWidget(QWidget * parent)
:GradientWidget(parent)
{
    d = new Private;
    gradient().setColorAt(0, "#E8E8E8");
    gradient().setColorAt(1, "#D0D0D0");
}

/*!
 * Deletes it.
 */
PlayPropertiesWidget::~PlayPropertiesWidget()
{
    delete d;
}

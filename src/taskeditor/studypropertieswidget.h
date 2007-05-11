//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef STUDYPROPERTIESWIDGET_H
#define STUDYPROPERTIESWIDGET_H

#include "gradientwidget.h"

/*!
 * \class StudyPropertiesWidget
 */
class StudyPropertiesWidget : public GradientWidget
{
Q_OBJECT
public:
    StudyPropertiesWidget(QWidget * parent = 0);
    virtual ~StudyPropertiesWidget();

private:
    class Private;
    Private * d;
};

#endif // STUDYPROPERTIESWIDGET_H

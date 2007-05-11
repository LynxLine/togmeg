//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef PLAYPROPERTIESWIDGET_H
#define PLAYPROPERTIESWIDGET_H

#include "gradientwidget.h"

/*!
 * \class PlayPropertiesWidget
 */
class PlayPropertiesWidget : public GradientWidget
{
Q_OBJECT
public:
    PlayPropertiesWidget(QWidget * parent = 0);
    virtual ~PlayPropertiesWidget();

public slots:
    void setCurrentTask(QString taskId);

private:
    class Private;
    Private * d;
};

#endif // PLAYPROPERTIESWIDGET_H

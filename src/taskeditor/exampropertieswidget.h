//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMPROPERTIESWIDGET_H
#define EXAMPROPERTIESWIDGET_H

#include "gradientwidget.h"

/*!
 * \class ExamPropertiesWidget
 */
class ExamPropertiesWidget : public GradientWidget
{
Q_OBJECT
public:
    ExamPropertiesWidget(QWidget * parent = 0);
    virtual ~ExamPropertiesWidget();

public slots:
    void setCurrentTask(QString taskId);

private slots:
    void randomizeStateChanged(int);

private:
    class Private;
    Private * d;
};

#endif // EXAMPROPERTIESWIDGET_H

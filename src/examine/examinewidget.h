//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMINEWIDGET_H
#define EXAMINEWIDGET_H

#include <QWidget>
#include "examinator.h"

class ExamineWidget : public QWidget
{
Q_OBJECT

public:
    ExamineWidget(QWidget * parent = 0L);
    virtual ~ExamineWidget();

    Examinator * examinator();

protected:
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void setTaskName(QString name);
    void setExaminatorState(Examinator::State);

private:
	class Private;
	Private * d;

};

#endif // EXAMINEWIDGET_H

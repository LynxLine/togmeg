//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMINEWIDGET_H
#define EXAMINEWIDGET_H

#include <QWidget>

/*!
 */
class ExamineWidget : public QWidget
{
Q_OBJECT

public:
    ExamineWidget(QWidget * parent = 0L);
    virtual ~ExamineWidget();

private:
	class Private;
	Private * d;

};

#endif // EXAMINEWIDGET_H

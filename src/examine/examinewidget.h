//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMINEWIDGET_H
#define EXAMINEWIDGET_H

#include <QWidget>
class Examinator;

class ExamineWidget : public QWidget
{
Q_OBJECT

public:
    ExamineWidget(QWidget * parent = 0L);
    virtual ~ExamineWidget();

    Examinator * examinator();

public slots:
    void setCurrentTask(QString taskId);

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
	class Private;
	Private * d;

};

#endif // EXAMINEWIDGET_H

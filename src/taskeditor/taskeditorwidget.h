//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORWIDGET_H
#define TASKEDITORWIDGET_H

#include <QWidget>

/*!
 */
class TaskEditorWidget : public QWidget
{
Q_OBJECT

public:
    TaskEditorWidget(QWidget * parent = 0L);
    virtual ~TaskEditorWidget();

public slots:
    void addNewEntry();
    void setCurrentTask(QString taskId);

private:
	class Private;
	Private * d;

};

#endif // TASKEDITORWIDGET_H

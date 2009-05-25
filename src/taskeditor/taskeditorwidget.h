//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORWIDGET_H
#define TASKEDITORWIDGET_H

#include <QWidget>

class TaskEditorView;
class StudyTaskModel;

/*!
 */
class TaskEditorWidget : public QWidget
{
Q_OBJECT

public:
    TaskEditorWidget(StudyTaskModel * model, QWidget * parent = 0L);
    virtual ~TaskEditorWidget();

    TaskEditorView * view() const;
    
public slots:
    void addNewEntry();

private:
	class Private;
	Private * d;

};

#endif // TASKEDITORWIDGET_H

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORWIDGET_H
#define TASKEDITORWIDGET_H

#include <QWidget>

class TaskEditorView;
class CramFileModel;

/*!
 */
class TaskEditorWidget : public QWidget
{
Q_OBJECT

public:
    TaskEditorWidget(CramFileModel * model, QWidget * parent = 0L);
    virtual ~TaskEditorWidget();

    TaskEditorView * view() const;
    
public slots:
    void addNewEntry();

private:
	class Private;
	Private * d;

};

#endif // TASKEDITORWIDGET_H

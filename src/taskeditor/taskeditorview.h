//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORVIEW_H
#define TASKEDITORVIEW_H

#include <QTreeView>

/*!
 * \class TaskEditorView
 */
class TaskEditorView : public QTreeView
{
Q_OBJECT
public:
    TaskEditorView(QWidget * parent = 0);
    virtual ~TaskEditorView();

public slots:
    void addNewEntry();
    void removeEntry();
    void openTask(QString taskId);

private slots:
    void activateContextMenu(const QPoint &);

private:
	class Private;
	Private * d;
};

#endif // TASKEDITORVIEW_H

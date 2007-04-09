//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKLISTVIEW_H
#define TASKLISTVIEW_H

#include <QTreeView>
class CategoryItem;

/*!
 * \class TaskListView
 */
class TaskListView : public QTreeView
{
Q_OBJECT
public:
    TaskListView(QWidget * parent = 0);
    virtual ~TaskListView();

private slots:
    void activateContextMenu(const QPoint &);

private:
	class Private;
	Private * d;
};

#endif // TASKLISTVIEW_H

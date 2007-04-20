//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKLISTVIEW_H
#define TASKLISTVIEW_H

#include <QTreeView>

/*!
 * \class TaskListView
 */
class TaskListView : public QTreeView
{
Q_OBJECT
public:
    TaskListView(QWidget * parent = 0);
    virtual ~TaskListView();

    QString currentTaskId();

public slots:
    void addNewStudy();
    void editCurrentStudy();
    void applyCategoryFilter(QString categoryId);

signals:
    void currentTaskChanged(QString taskId);
    void studyTaskActivated(QString taskId);

protected:
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);

private slots:
    void editStudyName();
    void activateItem(const QModelIndex &);
    void activateContextMenu(const QPoint &);

private:
	class Private;
	Private * d;
};

#endif // TASKLISTVIEW_H

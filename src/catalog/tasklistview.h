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
    void editStudyName();
    void editCurrentStudy();

signals:
    void currentTaskChanged(QString taskId);
    void studyTaskActivated(QString taskId);
    void rowCountChanged(int);

protected:
    virtual void paintEvent(QPaintEvent * pe);
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void activateItem(const QModelIndex &);
    void activateContextMenu(const QPoint &);

private:
	class Private;
	Private * d;
};

#include <itemdelegate.h>

class TaskListItemDelegate : public ItemDelegate
{
Q_OBJECT
public:
    TaskListItemDelegate(QAbstractItemView * parent = 0):ItemDelegate(parent) {;}
    
    //view
    virtual QSize sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //edit
    virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const;

};

#endif // TASKLISTVIEW_H

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
    void applyCategoryFilter(QString categoryId);

signals:
    void currentTaskChanged(QString taskId);
    void studyTaskActivated(QString taskId);
    void rowCountChanged(int);

protected:
    virtual void paintEvent(QPaintEvent * pe);
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);

private slots:
    void activateItem(const QModelIndex &);
    void activateContextMenu(const QPoint &);

private:
	class Private;
	Private * d;
};

#include <QItemDelegate>

class TaskListItemDelegate : public QItemDelegate
{
Q_OBJECT
public:
    TaskListItemDelegate(QObject * parent = 0):QItemDelegate(parent) {;}
    
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const {
        QStyleOptionViewItemV2 opt = option;
        opt.rect.setRect(opt.rect.x()+2,
                         opt.rect.y()+2,
                         opt.rect.width()-4,
                         opt.rect.height()-4);
        QItemDelegate::updateEditorGeometry(editor, opt, index);
    }

    virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        QSize s = QItemDelegate::sizeHint(option, index);
        s.setHeight(20);
        return s;
    }

};

#endif // TASKLISTVIEW_H

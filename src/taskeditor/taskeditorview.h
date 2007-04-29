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

protected:
    virtual void paintEvent(QPaintEvent * pe);
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	class Private;
	Private * d;
};

#include <QItemDelegate>

class TaskEditorItemDelegate : public QItemDelegate
{
Q_OBJECT
public:
    TaskEditorItemDelegate(QObject * parent = 0):QItemDelegate(parent) {;}
    
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const {
        QStyleOptionViewItemV2 opt = option;
        opt.rect.setRect(opt.rect.x()+1,
                         opt.rect.y()+1,
                         opt.rect.width()-2,
                         opt.rect.height()-2);
        QItemDelegate::updateEditorGeometry(editor, opt, index);
    }

    virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        QSize s = QItemDelegate::sizeHint(option, index);
        s.setHeight(20);
        return s;
    }

};

#endif // TASKEDITORVIEW_H

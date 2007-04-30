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
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
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
    TaskEditorItemDelegate(TaskEditorView * parent = 0):QItemDelegate(parent),view(parent) {;}
    
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const {
        QStyleOptionViewItemV2 opt = option;
        opt.rect.setRect(opt.rect.x()+1,
                         opt.rect.y()+2,
                         opt.rect.width()-1,
                         opt.rect.height()-3);
        QItemDelegate::updateEditorGeometry(editor, opt, index);

        QPalette vpalette = view->palette();
        QPalette palette = editor->palette();
        if (index.row() & 1) palette.setBrush(QPalette::Base, vpalette.brush(QPalette::AlternateBase));
        else palette.setBrush(QPalette::Base, vpalette.brush(QPalette::Base));
        editor->setPalette(palette);
    }

    virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        QSize s = QItemDelegate::sizeHint(option, index);
        s.setHeight(20);
        return s;
    }

    TaskEditorView * view;
};

#endif // TASKEDITORVIEW_H

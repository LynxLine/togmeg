//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORVIEW_H
#define TASKEDITORVIEW_H

#include <QTreeView>
#include "crammero.h"

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
    void toFirstRow();
    void activateContextMenu(const QPoint &);

protected:
    virtual void paintEvent(QPaintEvent * pe);
    virtual void keyPressEvent(QKeyEvent * ke);
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected slots:
    void editNextItem();
    void editPreviousItem();
    void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);

private:
	class Private;
	Private * d;
};

#include "itemdelegate.h"

class TaskEditorItemDelegate : public ItemDelegate
{
Q_OBJECT
public:
    TaskEditorItemDelegate(QObject * parent = 0):ItemDelegate(parent) {;}
    
    //view
    virtual QSize sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //edit
    virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //data
    virtual void setEditorData(QWidget * editor, const QModelIndex & i) const;
};

#endif // TASKEDITORVIEW_H

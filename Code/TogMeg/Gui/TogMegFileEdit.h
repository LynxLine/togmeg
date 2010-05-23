
#ifndef TASKEDITORVIEW_H
#define TASKEDITORVIEW_H

#include <QTreeView>
#include "TogMeg.h"

class TogMegFileModel;

/*!
 * \class TaskEditorView
 */
class TaskEditorView : public QTreeView
{
Q_OBJECT
public:
    enum NextItemMode {
        QAQAMode = 0,
        QQAAMode,
    };
    TaskEditorView(TogMegFileModel * model, QWidget * parent = 0);
    virtual ~TaskEditorView();
    NextItemMode nextItemMode();

    TogMegFileModel * studyTaskModel() const;
    
public slots:
    void addNewEntry();
    void removeEntry();
    void setNextItemMode(TaskEditorView::NextItemMode);

private slots:
    void toFirstRow();
    void activateContextMenu(const QPoint &);

signals:
    void nextItemModeChanged(TaskEditorView::NextItemMode);

protected:
    virtual void keyPressEvent(QKeyEvent * ke);
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected slots:
    void editNextItem();
    void editPreviousItem();
    void editNextItemUsingMode();
    void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);

private:
	class Private;
	Private * d;
};

#include "TogMegFileDelegate.h"

class TaskEditorItemDelegate : public ItemDelegate
{
Q_OBJECT
public:
    TaskEditorItemDelegate(QAbstractItemView * parent = 0):ItemDelegate(parent) {;}
    
    //view
    virtual QSize sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //edit
    virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //data
    virtual void setEditorData(QWidget * editor, const QModelIndex & i) const;

private slots:
    void editNextItem();
    void editPreviousItem();

};

#include <QLineEdit>

class TaskItemEditor : public QLineEdit
{
Q_OBJECT
public:
    TaskItemEditor(QWidget * parent = 0L):QLineEdit(parent) {;}

signals:
    void editNextItem();
    void editPreviousItem();

protected:
    virtual void keyPressEvent(QKeyEvent * ke);
};

#endif // TASKEDITORVIEW_H

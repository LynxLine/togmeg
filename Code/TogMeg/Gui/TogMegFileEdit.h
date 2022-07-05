
#ifndef TASKEDITORVIEW_H
#define TASKEDITORVIEW_H

#include <QTreeView>
#include "TogMeg.h"

class TogMegFileModel;

/*!
 * \class TogMegFileEdit
 */
class TogMegFileEdit : public QTreeView
{
Q_OBJECT
public:
    enum NextItemMode {
        QAQAMode = 0,
        QQAAMode,
    };
    TogMegFileEdit(TogMegFileModel * model, QWidget * parent = 0);
    virtual ~TogMegFileEdit();
    NextItemMode nextItemMode();

    TogMegFileModel * studyTaskModel() const;
    
public slots:
    void addNewEntry();
    void removeEntry();
    void swapQA();
    void setNextItemMode(TogMegFileEdit::NextItemMode);

private slots:
    void toFirstRow();
    void modelAboutToBeReset();
    void activateContextMenu(const QPoint &);

signals:
    void nextItemModeChanged(TogMegFileEdit::NextItemMode);

protected:
    virtual void keyPressEvent(QKeyEvent * ke);
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected slots:
    void editNextItem();
    void editAboveItem();
    void editBelowItem();
    void editPreviousItem();
    void editNextItemUsingMode();
    void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);

private:
	class Private;
	Private * d;
};

#include "TogMegFileDelegate.h"

class TaskEditorItemDelegate : public TogMegFileDelegate
{
Q_OBJECT
public:
    TaskEditorItemDelegate(QAbstractItemView * parent = 0):TogMegFileDelegate(parent) {;}
    
    //view
    virtual QSize sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //edit
    virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //data
    virtual void setEditorData(QWidget * editor, const QModelIndex & i) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * m, const QModelIndex & i) const;

signals:
    void gotoAboveItem();
    void gotoBelowItem();

private slots:
    void editNextItem();
    void editAboveItem();
    void editBelowItem();
    void editPreviousItem();

};

#include <QLineEdit>

class TaskItemEditor : public QLineEdit
{
Q_OBJECT
public:
    TaskItemEditor(QWidget * parent = 0L)
        :QLineEdit(parent), noAbove(false), noBelow(false)
    {setAlignment(Qt::AlignCenter);}

    void setNoAbove(bool);
    void setNoBelow(bool);

signals:
    void editNextItem();
    void editAboveItem();
    void editBelowItem();
    void editPreviousItem();

protected:
    virtual void keyPressEvent(QKeyEvent * ke);
    bool noAbove, noBelow;
};

#endif // TASKEDITORVIEW_H

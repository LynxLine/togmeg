//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QAbstractItemDelegate>

/*!
 * \class ItemDelegate
 */
class ItemDelegate : public QAbstractItemDelegate
{
Q_OBJECT
public:
    ItemDelegate(QObject * parent):QAbstractItemDelegate(parent) {;}
    virtual bool eventFilter(QObject * object, QEvent * event);

    //data
    virtual void setEditorData(QWidget * editor, const QModelIndex & i) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & i) const;

private slots:
    void commitDataAndCloseEditor(QWidget * editor);
};

#endif // ITEMDELEGATE_H

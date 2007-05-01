//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CATALOGVIEW_H
#define CATALOGVIEW_H

#include <QTreeView>
class CategoryItem;

/*!
 * \class CategoryView
 */
class CategoryView : public QTreeView
{
Q_OBJECT
public:
    CategoryView(QWidget * parent = 0);
    virtual ~CategoryView();

    void setModel(QAbstractItemModel * model);

public slots:
    void addNewCategory();
    void addSubCategory();
    void renameCategory();
    void removeCategory();

signals:
    void categoryActivated(QString id);

protected:
    virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void activateContextMenu(const QPoint &);
    void loadExpandState(CategoryItem * child);
    void saveExpandState(const QModelIndex & index);
    void saveCollapseState(const QModelIndex & index);
    void activateItem(const QModelIndex & index);

private:
	class Private;
	Private * d;
};

#include "itemdelegate.h"

class CategoryItemDelegate : public ItemDelegate
{
Q_OBJECT
public:
    CategoryItemDelegate(QObject * parent = 0):ItemDelegate(parent) {;}
    
    //view
    virtual QSize sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & o, const QModelIndex & i) const;

    //edit
    virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const;
};

#endif // CATALOGVIEW_H

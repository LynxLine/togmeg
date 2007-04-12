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
    void addSubCategory();
    void removeCategory();

signals:
    void categoryActivated(QString id);

protected:
    //virtual void drawBranches(QPainter *, const QRect &, const QModelIndex &) const;
    /*
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    */

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

#endif // CATALOGVIEW_H

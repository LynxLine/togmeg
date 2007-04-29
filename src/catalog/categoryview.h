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
    //virtual void drawBranches(QPainter *, const QRect &, const QModelIndex &) const;
    
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

#include <QItemDelegate>

class CategoryItemDelegate : public QItemDelegate
{
Q_OBJECT
public:
    CategoryItemDelegate(QObject * parent = 0):QItemDelegate(parent) {;}
    
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const {
        QStyleOptionViewItemV2 opt = option;
        opt.rect.setRect(opt.rect.x()+2,
                         opt.rect.y()+2,
                         opt.rect.width()-4,
                         opt.rect.height()-4);
        QItemDelegate::updateEditorGeometry(editor, opt, index);
    }

};

#endif // CATALOGVIEW_H

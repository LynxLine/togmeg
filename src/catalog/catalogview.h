//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CATALOGVIEW_H
#define CATALOGVIEW_H

#include <QTreeView>

/*!
 * \class CatalogView
 */
class CatalogView : public QTreeView
{
Q_OBJECT
public:
    CatalogView(QWidget * parent = 0);
    virtual ~CatalogView();

protected:
    /*
    virtual void drawBranches(QPainter *, const QRect &, const QModelIndex &) const;
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    */

private slots:
    void activateContextMenu(const QPoint &);

private:
	class Private;
	Private * d;
};

#endif // CATALOGVIEW_H

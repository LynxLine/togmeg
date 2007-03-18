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
    CatalogView(QWidget * parent = 0):QTreeView(parent) {;}


protected:
    virtual void drawBranches(QPainter *, const QRect &, const QModelIndex &) const {;}
    
};

#endif // CATALOGVIEW_H

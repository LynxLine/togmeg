//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>

/*!
 */
class CatalogWidget : public QWidget
{
Q_OBJECT

public:
    CatalogWidget(QWidget * parent = 0L);
    virtual ~CatalogWidget();

private:
	class Private;
	Private * d;

};

#endif // CATALOGWIDGET_H

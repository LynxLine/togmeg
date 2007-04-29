//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QHeaderView>

/*!
 * \class HeaderView
 */
class HeaderView : public QHeaderView
{
Q_OBJECT
public:
    HeaderView(Qt::Orientation orientation, QWidget * parent = 0);
    virtual ~HeaderView();

protected:
    virtual QSize sectionSizeFromContents(int logicalIndex) const;
    virtual void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const;

private:
    class Private;
    Private * d;
};

#endif // HEADERVIEW_H

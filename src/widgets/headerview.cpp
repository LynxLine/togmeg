//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "headerview.h"

class HeaderView::Private {
public:
    QPixmap pm;
};

/*!
 * Creates the object.
 */
HeaderView::HeaderView(Qt::Orientation orientation, QWidget * parent)
:QHeaderView(orientation, parent)
{
    d = new Private;
    d->pm = QPixmap(":/images/bg-header-view-horizontal.png");

    //if ( orientation == Qt::Horizontal ) {
    //}
}

/*!
 * Deletes it.
 */
HeaderView::~HeaderView()
{
    delete d;
}

QSize HeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize s = QHeaderView::sectionSizeFromContents(logicalIndex);
    if ( orientation() == Qt::Horizontal ) {
        s.setHeight( 16 );
    }
    return s;
}

void HeaderView::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
    if ( orientation() == Qt::Horizontal ) {
        // background
        for (int i=0;i<rect.width()/d->pm.width()+1;i++) {
            painter->drawPixmap(rect.x() +i*d->pm.width(), rect.y(), 
                               d->pm.width(), rect.height(), 
                               d->pm
            );
        }

        if ( logicalIndex ) {
            QRect r = rect;
            painter->setPen(QPen(QColor("#C0C0C0"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawLine(r.x(), r.y(), r.x(), r.y()+r.height());
        }

        if ( model() ) {
            QVariant textAlignment = model()->headerData(logicalIndex, orientation(), Qt::TextAlignmentRole);
            Qt::Alignment align = Qt::Alignment(textAlignment.isValid()
                                    ? Qt::Alignment(textAlignment.toInt())
                                    : Qt::AlignLeft );
            align |= Qt::AlignVCenter;

            int margin = 3;
            if (logicalIndex) margin++;

            QRect r = rect;
            r.setX( r.x()+margin );
            r.setHeight( r.height() -1 );
            r.setWidth( r.width() -2*margin );

            QString headerName = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();
            painter->setPen(QPen(QColor("#000000"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawText(r, align, headerName);
        }
    }
    else
        QHeaderView::paintSection(painter, rect, logicalIndex);
}


#include <QtGui>
#include "VHeader.h"

#include "AppStyles.h"
#include "ColumnSelectorButton.h"

QVHeaderView::QVHeaderView(Qt::Orientation o, QWidget * parent)
:QHeaderView(o, parent)
{
    setClickable(true);
    setHighlightSections(true);
}

QVHeaderView::~QVHeaderView()
{
}

void QVHeaderView::paintSection(QPainter * p, const QRect & r, int idx) const
{
    QRect rect = r;

    Qt::Orientation o = orientation();
    QStyleOptionHeader opt;
    initStyleOption(&opt);

    if (o == Qt::Horizontal) {
        QPoint c = r.center();

        QTransform tf;
        tf.translate(c.x(), c.y());
        tf.rotate(-90);
        tf.translate(-c.x(), -c.y());

        QRect rm = tf.mapRect(r);

        p->save();
        p->setTransform(tf);

        bool wf = r.width() % 2;
        bool hf = r.height() % 2;

        rm.translate(-1,1);
        if (!wf) rm.translate(0,1);
        if (!hf) rm.translate(-1,0);

        rect = rm;

        if (selectionModel()->columnIntersectsSelection(idx, QModelIndex())) {
            opt.state |= QStyle::State_On;
        }
    }
    else {
        if (selectionModel()->rowIntersectsSelection(idx, QModelIndex())) {
            opt.state |= QStyle::State_On;
        }

    }

    if (opt.state & QStyle::State_Selected)
        p->fillRect(rect, QColor("#5497d9"));
    else if (opt.state & QStyle::State_On)
        p->fillRect(rect, QColor("#bfd5ea"));
    else p->fillRect(rect, QColor("#f0f0f0"));

    p->setPen(QColor("#a0a0a0"));

    if (o == Qt::Horizontal) {
        p->drawLine(rect.topLeft(), rect.bottomLeft());
        p->drawLine(rect.bottomLeft(), rect.bottomRight());
    }
    else {
        p->drawLine(rect.topRight(), rect.bottomRight());
        p->drawLine(rect.bottomLeft(), rect.bottomRight());
    }

    rect.adjust(3,0,-3,0);
    QString text = selectionModel()->model()->headerData(idx, o).toString();
    QColor c = selectionModel()->model()->headerData(idx, o, Qt::ForegroundRole).value<QColor>();

    p->setPen(c);
    p->drawText(rect, text, Qt::AlignHCenter | Qt::AlignVCenter);

    if (o == Qt::Horizontal) {
        p->restore();
    }
}

QSize QVHeaderView::sectionSizeFromContents(int idx) const {
    Qt::Orientation o = orientation();
    if (o == Qt::Horizontal) {
        QSize s = QHeaderView::sectionSizeFromContents(idx);
        s.transpose();
        return s;
    }
    return QHeaderView::sectionSizeFromContents(idx);
}


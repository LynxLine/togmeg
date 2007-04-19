//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "roundedgradientwidget.h"

class RoundedGradientWidget::Private {
public:
    QGradient gradient;
    QFont windowTitleFont;
    qreal roundRadius;
};

/*!
 * Creates the object.
 */
RoundedGradientWidget::RoundedGradientWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    d->roundRadius = 20.0;
    d->gradient = QLinearGradient();
}

/*!
 * Deletes it.
 */
RoundedGradientWidget::~RoundedGradientWidget()
{
    delete d;
}

void RoundedGradientWidget::paintEvent(QPaintEvent * pe)
{
    QPainter p(this);
    p.setClipRegion(pe->region());
    p.setRenderHint(QPainter::Antialiasing);

    qreal r = d->roundRadius;
    qreal w = qreal(width()) -2.0;
    qreal h = qreal(height()) -2.0;

    //path
    QPainterPath path;
    path.moveTo(w/2, 0);
    path.arcTo(w-2*r, 0, 2*r, 2*r, 90, -90);
    path.arcTo(w-2*r, h-2*r, 2*r, 2*r, 0, -90);
    path.arcTo(0, h-2*r, 2*r, 2*r, -90, -90);
    path.arcTo(0, 0, 2*r, 2*r, -180, -90);

    path.closeSubpath();

    //paint path
    p.translate(1.0, 1.0);
    p.setPen(QPen(QColor("#C2C2C2"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        
    p.setBrush(d->gradient);
    p.drawPath(path);

    p.setPen(QPen(QColor("#ffffff"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.setFont( windowTitleFont() );
    p.drawText(20,20, windowTitle());
}


void RoundedGradientWidget::resizeEvent(QResizeEvent * re)
{
    if (d->gradient.type() == QGradient::LinearGradient) {
        QLinearGradient * linearGradient = (QLinearGradient *)&d->gradient;
        linearGradient->setStart(QPointF(0, 0));
        linearGradient->setFinalStop(QPointF(0, re->size().height()));
    }
    QWidget::resizeEvent(re);
}

QGradient & RoundedGradientWidget::gradient()
{
    return d->gradient;
}

void RoundedGradientWidget::setGradient(QGradient g)
{
    d->gradient = g;
}

QFont RoundedGradientWidget::windowTitleFont()
{
    return d->windowTitleFont;
}

void RoundedGradientWidget::setWindowTitleFont(QFont f)
{
    d->windowTitleFont = f;
}

qreal RoundedGradientWidget::roundRadius()
{
    return d->roundRadius;
}

void RoundedGradientWidget::setRoundRadius(qreal r)
{
    d->roundRadius = r;
}

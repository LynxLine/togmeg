
#include <QtGui>
#include "gradientwidget.h"

class GradientWidget::Private {
public:
    QGradient gradient;
};

/*!
 * Creates the object.
 */
GradientWidget::GradientWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    setAutoFillBackground(true);
    d->gradient = QLinearGradient();
}

/*!
 * Deletes it.
 */
GradientWidget::~GradientWidget()
{
    delete d;
}

void GradientWidget::resizeEvent(QResizeEvent * re)
{
    if (d->gradient.type() == QGradient::LinearGradient) {
        QLinearGradient * linearGradient = (QLinearGradient *)&d->gradient;
        linearGradient->setStart(QPointF(0, 0));
        linearGradient->setFinalStop(QPointF(0, re->size().height()));

        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, d->gradient);
        setPalette(palette);
    }
    QWidget::resizeEvent(re);
}

QGradient & GradientWidget::gradient()
{
    return d->gradient;
}


#ifndef ROUNDEDGRADIENTWIDGET_H
#define ROUNDEDGRADIENTWIDGET_H

#include <QWidget>

/*!
 * \class RoundedGradientWidget
 */
class GradientWidget : public QWidget
{
Q_OBJECT
public:
    GradientWidget(QWidget * parent = 0);
    virtual ~GradientWidget();

    QGradient & gradient();
    void setGradient(QGradient);

    QFont windowTitleFont();
    void setWindowTitleFont(QFont);

    qreal roundRadius();
    void setRoundRadius(qreal);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

private:
    class Private;
    Private * d;
};

#endif // ROUNDEDGRADIENTWIDGET_H

#ifndef ROUNDEDGRADIENTWIDGET_H
#define ROUNDEDGRADIENTWIDGET_H

#include <QWidget>

/*!
 * \class GradientWidget
 */
class GradientWidget : public QWidget
{
Q_OBJECT
public:
    GradientWidget(QWidget * parent = 0);
    virtual ~GradientWidget();
    QGradient & gradient();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    class Private;
    Private * d;
};

#endif // ROUNDEDGRADIENTWIDGET_H

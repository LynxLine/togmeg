//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef ROUNDEDGRADIENTWIDGET_H
#define ROUNDEDGRADIENTWIDGET_H

#include <QWidget>

/*!
 * \class RoundedGradientWidget
 */
class RoundedGradientWidget : public QWidget
{
Q_OBJECT
public:
    RoundedGradientWidget(QWidget * parent = 0);
    virtual ~RoundedGradientWidget();

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

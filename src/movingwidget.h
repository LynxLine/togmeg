// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef MOVINGWIDGET_H
#define MOVINGWIDGET_H

#include <QWidget>

/*!
 * \class MovingWidget
 */
class MovingWidget : public QWidget
{
Q_OBJECT
public:
    MovingWidget(QWidget * parent = 0);
    ~MovingWidget();

    void setMovingTime(int msecs);
    void setMovingEffect(QPoint from, QPoint to);
    
public slots:
    void moveForward();
    void moveBackward();

private slots:
    void moving();
 
private:
    class Private;
    Private * d;
};

#endif // MOVINGWIDGET_H


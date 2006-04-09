// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef BOXWINDOW_H
#define BOXWINDOW_H

#include <QWidget>

/*!
 \class BoxWindow
 */
class BoxWindow : public QWidget
{
Q_OBJECT
public:
    enum Style { Question = 0, Answer };
    BoxWindow(QWidget * parent = 0);
    ~BoxWindow();
    
protected:
    virtual void paintEvent(QPaintEvent * pe);
    
private:
    class Private;
    Private * d;
};

#endif // BOXWINDOW_H


//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>

/*!
 * \class TabBar
 */
class TabBar : public QWidget
{
Q_OBJECT
public:
    TabBar(QWidget * parent = 0);
    virtual ~TabBar();

    int addTab(QString);
    int currentIndex();
    int count();

signals:
    void currentChanged(int);

protected:
    virtual void paintEvent(QPaintEvent * pe);
    virtual void mousePressEvent(QMouseEvent * me);
    virtual void mouseReleaseEvent(QMouseEvent * me);

private:
    class Private;
    Private * d;
};

#endif // TABBAR_H

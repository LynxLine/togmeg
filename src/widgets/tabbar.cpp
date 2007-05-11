//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "tabbar.h"

class TabBar::Private {
public:
    Private(TabBar * parent)
    :p(parent),
     pmBg(":/images/bg-tab.png"),
     pmActiveTab(":/images/tab-on.png"),
     pmInActiveTab(":/images/tab.png"),
     current(-1),
     pressed(-1)
    {;}
    TabBar * p;
    QPixmap pmBg;
    QPixmap pmActiveTab;
    QPixmap pmInActiveTab;

    int current;
    int pressed;
    QStringList tabs;

    QRect tabRect(int i);
};

/*!
 * Creates the object.
 */
TabBar::TabBar(QWidget * parent)
:QWidget(parent)
{
    d = new Private(this);

    Q_ASSERT( !d->pmBg.isNull() );
    Q_ASSERT( !d->pmActiveTab.isNull() );
    Q_ASSERT( !d->pmInActiveTab.isNull() );

    setFixedHeight( d->pmBg.height() );
}

/*!
 * Deletes it.
 */
TabBar::~TabBar()
{
    delete d;
}

int TabBar::addTab(QString tab)
{
    d->tabs << tab;
    if (d->current < 0)
        d->current = 0;
    return count()-1;
}

int TabBar::currentIndex()
{
    return d->current;
}

int TabBar::count()
{
    return d->tabs.count();
}

QRect TabBar::Private::tabRect(int i)
{
    int w = p->width();
    int x = (w - tabs.count() * (pmActiveTab.width()+3))/2;
    x += i*(pmActiveTab.width()+3);
    return QRect(QPoint(x,0), pmActiveTab.size());
}

void TabBar::paintEvent(QPaintEvent * pe)
{
    QPainter p(this);
    p.setClipRect(pe->rect());
    
    int w = width();

    int i;
    for (i=0;i< w/d->pmBg.width() +1;i++) {
        p.drawPixmap(i*d->pmBg.width(), 0, d->pmBg );
    }

    for (i=0;i<count();i++) {
        if ( i== d->current || i== d->pressed)  
            p.drawPixmap(d->tabRect(i), d->pmActiveTab);
        else p.drawPixmap(d->tabRect(i), d->pmInActiveTab);

        QRect r = d->tabRect(i);
        r.adjust(2,11,-2,1);

        p.setPen(QColor("#F0F0F0"));
        p.drawText(r, Qt::AlignCenter, d->tabs[i]);

        r.adjust(0,-1,0,-1);
        p.setPen(QColor("#505050"));
        p.drawText(r, Qt::AlignCenter, d->tabs[i]);
    }
}

void TabBar::mousePressEvent(QMouseEvent * me)
{
    QWidget::mousePressEvent(me);

    for (int i=0;i<count();i++) {
        if ( d->tabRect(i).contains(me->pos()) ) {
            d->pressed = i;
            update();
            break;
        }
    }
}

void TabBar::mouseReleaseEvent(QMouseEvent * me)
{
    QWidget::mouseReleaseEvent(me);
    for (int i=0;i<count();i++) {
        if ( d->tabRect(i).contains(me->pos()) && i == d->pressed) {
            d->current = d->pressed;
            d->pressed = -1;
            update();

            emit currentChanged( currentIndex() );
            break;
        }
    }
}

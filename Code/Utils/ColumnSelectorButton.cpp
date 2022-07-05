//
// Copyright Petrostreamz 2009
//

#include <QtGui>
#include "ColumnSelectorButton.h"

#include "IconSet.h"
#include "AppStyles.h"

class ColumnSelectorButton::Private {
public:
    QPointer<QHeaderView> header;
    QPointer<QMenu> menu;
    QPixmap pm;    
};

ColumnSelectorButton::ColumnSelectorButton(QHeaderView * header, QWidget * parent)
:QToolButton(parent)
{
    d = new Private;
    d->header = header;
    d->pm = EmbPixmap("HeaderDropDown");
    setFocusPolicy(Qt::NoFocus);
}

ColumnSelectorButton::~ColumnSelectorButton()
{
    delete d;
}

void ColumnSelectorButton::setPixmap(QPixmap pm)
{
    d->pm = pm;
}

void ColumnSelectorButton::mouseDoubleClickEvent(QMouseEvent * me)
{
    Q_UNUSED(me);
    emit doubleClicked();
}

void ColumnSelectorButton::paintEvent(QPaintEvent * pe)
{
    int w = width();
    int h = height();
#ifdef Q_WS_MAC
    w++;
#endif
    
    QPainter p(this);
    p.setClipRect(pe->rect());
    
    QStyleOptionHeader opt;
    opt.initFrom(d->header);
    opt.state |= QStyle::State_Horizontal | QStyle::State_Enabled | QStyle::State_Raised;
    opt.rect = QRect(0,0, w,h);
    style()->drawControl(QStyle::CE_Header, &opt, &p, d->header);
    
#ifdef Q_WS_MAC
    p.setPen(QColor("#a0a0a0"));
    p.drawLine(0, h-1, w, h-1);
#endif
    
    p.drawPixmap((w- d->pm.width())/2, (h- d->pm.height())/2, d->pm);
    p.end();
}

void ColumnSelectorButton::showContentMenu()
{
    if (!d->menu) return;
    
    int menuWidth = d->menu->sizeHint().width();
    QPoint pos(width()-menuWidth, height());
    pos = mapToGlobal(pos);
    
    d->menu->popup(pos);
    
    menuWidth = d->menu->sizeHint().width();
    pos = QPoint(width()-menuWidth, height());
    pos = mapToGlobal(pos);
    d->menu->move(pos);
}

void ColumnSelectorButton::setContentMenu(QMenu * menu)
{
    d->menu = menu;
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "pixmapbutton.h"

class PixmapButton::Private {
public:
    Private():highlighted(false) {;}
    bool highlighted;

    QPixmap pm;
    QPixmap pmHl;
    QPixmap pmDown;
};

PixmapButton::PixmapButton(QWidget * parent)
:QAbstractButton(parent)
{
    d = new Private;    
    setFocusPolicy(Qt::NoFocus);
}

PixmapButton::PixmapButton(QString pm, QString pmHl, QString pmDown, QWidget * parent)
:QAbstractButton( parent )	
{
    d = new Private;

    if ( !d->pm.load( pm ) )
		qFatal("Failed to load button %s", pm.toLocal8Bit().data());

    if ( !d->pmHl.load( pmHl ) )
		qFatal("Failed to load button %s", pmHl.toLocal8Bit().data());

	if ( !d->pmDown.load( pmDown ) )
		qFatal("Failed to load button %s", pmDown.toLocal8Bit().data());
	
    Q_ASSERT( d->pmHl.size() == d->pm.size() );
	Q_ASSERT( d->pmDown.size() == d->pm.size() );
    
    setFixedSize(d->pm.size());
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);
}

PixmapButton::~PixmapButton()
{
    delete d;
}

void PixmapButton::enterEvent(QEvent * e)
{
	d->highlighted = true;
    QAbstractButton::enterEvent(e);
	update();

    emit highlighted(d->highlighted);	
}

void PixmapButton::leaveEvent(QEvent * e)
{
	d->highlighted = false;
	QAbstractButton::leaveEvent(e);
	update();

    emit highlighted(d->highlighted);
}

void PixmapButton::paintEvent(QPaintEvent * pe)
{
	QPainter p(this);
    p.setClipRegion(pe->region());
    if ( d->highlighted ) {
        if ( isDown() ) p.drawPixmap(0,0, d->pmDown );
        else            p.drawPixmap(0,0, d->pmHl );
    } 
    else {
        if(isChecked()) {
                p.drawPixmap(0,0, d->pmDown );
            } else p.drawPixmap(0,0, d->pm );
    }
	p.end();
}

bool PixmapButton::isHighlighted()
{
    return d->highlighted;
}

void PixmapButton::setHighlighted(bool f)
{
    d->highlighted = f;
    update();
}

QPixmap PixmapButton::pixmap()
{
    return d->pm;
}

QPixmap PixmapButton::pixmapHl()
{
    return d->pmHl;
}

QPixmap PixmapButton::pixmapDown()
{
    return d->pmDown;
}

void PixmapButton::setPixmap(QPixmap pm)
{
    setFixedSize(pm.size());
    d->pm = pm;
}

void PixmapButton::setPixmapHl(QPixmap pm)
{
    d->pmHl = pm;
}

void PixmapButton::setPixmapDown(QPixmap pm)
{
    d->pmDown = pm;
}

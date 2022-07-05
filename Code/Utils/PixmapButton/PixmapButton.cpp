
#include <QtGui>
#include "PixmapButton.h"

class PixmapButton::Private {
public:
    Private():highlighted(false) {;}
    bool highlighted;

    QPixmap pm;
    QPixmap pmHl;
    QPixmap pmOff;
    QPixmap pmDown;
};

PixmapButton::PixmapButton(QWidget * parent)
:QAbstractButton(parent)
{
    d = new Private;    
    setFocusPolicy(Qt::NoFocus);
}

PixmapButton::PixmapButton(QPixmap pm, QPixmap pmHl, QPixmap pmDown, QPixmap pmOff, QWidget * parent)
:QAbstractButton( parent )	
{
    d = new Private;

    d->pm = pm;
    d->pmHl = pmHl;
    if (pmOff.isNull())
        d->pmOff = pm;
    else d->pmOff = pmOff;
    d->pmDown = pmDown;
    
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
    
    if (isEnabled()) {
        if ( d->highlighted ) {
            if ( isDown() ) p.drawPixmap(0,0, d->pmDown );
            else            p.drawPixmap(0,0, d->pmHl );
        }
        else p.drawPixmap(0,0, d->pm );
    }
    else p.drawPixmap(0,0, d->pmOff );
	p.end();
}

void PixmapButton::mousePressEvent(QMouseEvent * me)
{
    QAbstractButton::mousePressEvent(me);
}

void PixmapButton::mouseReleaseEvent(QMouseEvent * me)
{
    QAbstractButton::mouseReleaseEvent(me);
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
    d->pmOff = pm;
    d->pm = pm;
}

void PixmapButton::setPixmapHl(QPixmap pm)
{
    d->pmHl = pm;
}

void PixmapButton::setPixmapOff(QPixmap pm)
{
    d->pmOff = pm;
}

void PixmapButton::setPixmapDown(QPixmap pm)
{
    d->pmDown = pm;
}

QSize PixmapButton::sizeHint() const
{
    return minimumSizeHint();
}

QSize PixmapButton::minimumSizeHint() const
{
    return d->pm.size();
}


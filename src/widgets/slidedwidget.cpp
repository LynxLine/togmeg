//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "slidedwidget.h"

#define SLIDEDWIDGET_FRAMES 25

class SlidedWidget::Private {
public:
    SlidedWidget * p;
    QList<QWidget *> widgets;
    int currentIndex;
    int oldIndex;
    QTimeLine * timeLine;

    void relayout(QSize s);
};

/*!
 * Creates the object.
 */
SlidedWidget::SlidedWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    d->currentIndex = -1;
    d->p = this;

    d->timeLine = new QTimeLine(300, this);
    d->timeLine->setFrameRange(0, SLIDEDWIDGET_FRAMES);
    d->timeLine->setLoopCount(1);
    connect(d->timeLine, SIGNAL(frameChanged(int)), 
            this, SLOT(animateSwitching(int)));
    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(animateFinished()));
}

/*!
 * Deletes it.
 */
SlidedWidget::~SlidedWidget()
{
    delete d;
}

void SlidedWidget::addWidget(QWidget * widget)
{
    if ( d->currentIndex < 0)  
        d->currentIndex = 0;

    d->widgets << widget;
    d->relayout( size() );
}

int SlidedWidget::count() const
{
    return d->widgets.count();
}

int SlidedWidget::currentIndex() const
{
    return d->currentIndex;
}

QWidget * SlidedWidget::currentWidget() const
{
    return d->widgets[ d->currentIndex ];
}

int SlidedWidget::indexOf(QWidget * widget) const
{
    for (int i=0;i<count();i++) {
        if ( widget == d->widgets[i] ) 
            return i;
    }

    return -1;
}

QWidget * SlidedWidget::widget(int index) const
{
    return d->widgets[ index ];
}

void SlidedWidget::setCurrentIndex(int index)
{
    if ( index == d->currentIndex ) return;
    setFocusProxy( d->widgets[ index ] );
    d->oldIndex = d->currentIndex;
    d->currentIndex = index;
    d->timeLine->start();
}

void SlidedWidget::setCurrentWidget(QWidget * widget)
{
    setCurrentIndex( indexOf( widget ) );
}

void SlidedWidget::resizeEvent(QResizeEvent * re)
{
    QWidget::resizeEvent(re);
    d->relayout( re->size() );
}

void SlidedWidget::Private::relayout(QSize s)
{
    if ( currentIndex < 0 ) return;
    for (int i=0;i<widgets.count();i++) {
        int x = (i -currentIndex) * s.width();
        int y = 0;
        widgets[i]->setGeometry(x,y, s.width(), s.height());
        widgets[i]->setEnabled( i == currentIndex );
    }
}

void SlidedWidget::animateSwitching(int frame)
{
    QSize s = size();
    for (int i=0;i<count();i++) {
        int x = (i -d->oldIndex) * s.width();
            x += (d->oldIndex - d->currentIndex) * s.width() * frame/SLIDEDWIDGET_FRAMES;
        int y = 0;
        d->widgets[i]->setGeometry(x,y, s.width(), s.height());
        d->widgets[i]->setEnabled( true );
    }
}

void SlidedWidget::animateFinished()
{
    d->relayout( size() );
    currentWidget()->setFocus();
    emit currentChanged(d->currentIndex);
}

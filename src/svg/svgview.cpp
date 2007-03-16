/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "svgview.h"

#include <QSvgRenderer>

#include <QApplication>
#include <QPainter>
#include <QImage>
#include <QWheelEvent>
#include <QtDebug>

SvgRasterView::SvgRasterView(const QString &file, QWidget *parent)
    : QWidget(parent)
{
    doc = new QSvgRenderer(file, this);
    connect(doc, SIGNAL(repaintNeeded()),
            this, SLOT(poluteImage()));
}

void SvgRasterView::paintEvent(QPaintEvent *)
{
    if (buffer.size() != size() ||
        m_dirty) {
        buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);
        QPainter p(&buffer);
        p.setViewport(0, 0, width(), height());
        p.eraseRect(0, 0, width(), height());
        doc->render(&p);
    }
    QPainter pt(this);
    pt.drawImage(0, 0, buffer);
}

QSize SvgRasterView::sizeHint() const
{
    if (doc)
        return doc->defaultSize();
    return QWidget::sizeHint();
}


void SvgRasterView::poluteImage()
{
    m_dirty = true;
    update();
}

void SvgRasterView::wheelEvent(QWheelEvent *e)
{
    const double diff = 0.1;
    QSize size = doc->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (e->delta() > 0) {
        width = int(this->width()+this->width()*diff);
        height = int(this->height()+this->height()*diff);
    } else {
        width  = int(this->width()-this->width()*diff);
        height = int(this->height()-this->height()*diff);
    }

    resize(width, height);
}

SvgNativeView::SvgNativeView(const QString &file, QWidget *parent)
    : QWidget(parent)
{
    doc = new QSvgRenderer(file, this);
    connect(doc, SIGNAL(repaintNeeded()),
            this, SLOT(update()));
}

void SvgNativeView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setViewport(0, 0, width(), height());
    doc->render(&p);
}

QSize SvgNativeView::sizeHint() const
{
    if (doc)
        return doc->defaultSize();
    return QWidget::sizeHint();
}

void SvgNativeView::wheelEvent(QWheelEvent *e)
{
    const double diff = 0.1;
    QSize size = doc->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (e->delta() > 0) {
        width = int(this->width()+this->width()*diff);
        height = int(this->height()+this->height()*diff);
    } else {
        width  = int(this->width()-this->width()*diff);
        height = int(this->height()-this->height()*diff);
    }
    resize(width, height);
}

#ifndef QT_NO_OPENGL
SvgGLView::SvgGLView(const QString &file, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    doc = new QSvgRenderer(file, this);
    connect(doc, SIGNAL(repaintNeeded()),
            this, SLOT(update()));
}

void SvgGLView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    doc->render(&p);
}

QSize SvgGLView::sizeHint() const
{
    if (doc)
        return doc->defaultSize();
    return QGLWidget::sizeHint();
}

void SvgGLView::wheelEvent(QWheelEvent *e)
{
    const double diff = 0.1;
    QSize size = doc->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (e->delta() > 0) {
        width = int(this->width()+this->width()*diff);
        height = int(this->height()+this->height()*diff);
    } else {
        width  = int(this->width()-this->width()*diff);
        height = int(this->height()-this->height()*diff);
    }
    resize(width, height);
}
#endif

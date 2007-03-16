//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>

#include "svgview.h"
#include "svgwidget.h"

SvgWidget::SvgWidget(QWidget * parent)
:QWidget(parent)
{
    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    currentPath = "C:\\bubbles.svg";
    setRenderer(OpenGL);
}

void SvgWidget::openFile(const QString &file)
{
    currentPath = file;
    setRenderer(renderer);
}

void SvgWidget::setRenderer(RendererType type)
{
    renderer = type;
    QWidget * view;

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0L) {
        delete child;
    }

    if (renderer == OpenGL)
        view = new SvgGLView(currentPath, this);
    else if (renderer == Image)
        view = new SvgRasterView(currentPath, this);
    else
        view = new SvgNativeView(currentPath, this);

    layout->addWidget(view);
}

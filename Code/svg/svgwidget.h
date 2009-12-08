//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>

class SvgWidget : public QWidget
{
public:
    enum RendererType { Native, OpenGL, Image };
    SvgWidget(QWidget * parent);

    void openFile(const QString &file);
    void setRenderer(RendererType type = Native);

private:
    QHBoxLayout * layout;
    QString currentPath;
    RendererType renderer;
};

#endif //SVGWIDGET_H

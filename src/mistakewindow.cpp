// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "mistakewindow.h"

class MistakeWindow::Private {
public:
    Private():
        pmTop        (":/images/red-box/top.png"),
        pmLeft       (":/images/red-box/left.png"),
        pmRight      (":/images/red-box/right.png"),
        pmBottom     (":/images/red-box/bottom.png"),
        pmLeftTop    (":/images/red-box/top-left.png"),
        pmRightTop   (":/images/red-box/top-right.png"),
        pmLeftBottom (":/images/red-box/bottom-left.png"),
        pmRightBottom(":/images/red-box/bottom-right.png")
    {}

    QPixmap pmTop;
    QPixmap pmLeft;
    QPixmap pmRight;
    QPixmap pmBottom;
    QPixmap pmLeftTop;
    QPixmap pmRightTop;
    QPixmap pmLeftBottom;
    QPixmap pmRightBottom;

    QLabel * l_message;
};

/*!
 * Creates the object.
 */
MistakeWindow::MistakeWindow(QWidget * parent)
:MovingWidget(parent)
{
    d = new Private;
    setVisible(false);

    QWidget *desktop = qApp->activeWindow();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 400) / 800;
    int h = (height* 200) / 600;
    resize(w,h);
    
    int x = (width * 200) / 800;
    int y = (height* 150) / 600;
    move(x,y);

    setContentsMargins(d->pmLeftTop.width(),     d->pmLeftTop.height(), 
                       d->pmRightBottom.width(), d->pmRightBottom.height());
    
    setAutoFillBackground(false);

    setMinimumWidth ( d->pmLeftTop.width() +1+d->pmRightBottom.width() );
    setMinimumHeight( d->pmLeftTop.height()+1+d->pmRightBottom.height() );

    int b = (height*  25) / 600;
    setContentsMargins(b, b, b, b);

    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (b * 75 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    d->l_message = new QLabel;
    d->l_message->setFont(font);
    
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(d->l_message,  0,0);
    grid->setSpacing(0);
    grid->setMargin(0);
    setLayout(grid);
}

/*!
 * Deletes it.
 */
MistakeWindow::~MistakeWindow()
{
    delete d;
}

void MistakeWindow::paintEvent(QPaintEvent * pe)
{
    int w = width();
    int h = height();

    int w1 = d->pmLeftTop.width();
    int w2 = d->pmRightBottom.width();
    
    int h1 = d->pmLeftTop.height();
    int h2 = d->pmRightBottom.height();
    
    QPainter p(this);
    p.setClipRegion( pe->region() );
    
    p.drawPixmap(0,    0,    w1, h1, d->pmLeftTop);
    p.drawPixmap(0,    h-h2, w1, h2, d->pmLeftBottom);
    p.drawPixmap(w-w2, 0,    w2, h1, d->pmRightTop);
    p.drawPixmap(w-w2, h-h2, w2, h2, d->pmRightBottom);

    p.drawPixmap(0,    h1,   w1,      h-h1-h2, d->pmLeft);
    p.drawPixmap(w1,   0,    w-w1-w2, h1,      d->pmTop);
    p.drawPixmap(w-w2, h1,   w2,      h-h1-h2, d->pmRight);
    p.drawPixmap(w1,   h-h2, w-w1-w2, h2,      d->pmBottom);
    
    p.fillRect(w1,h1, w-w1-w2, h-h1-h2, QColor("#ffbebe"));
    p.end();
    
    QWidget::paintEvent(pe);
}

void MistakeWindow::setText(QString text)
{
    d->l_message->setText(text);
}


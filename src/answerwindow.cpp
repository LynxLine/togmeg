// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "answerwindow.h"

#define SCROLL_MSECS 200

class AnswerWindow::Private {
public:
    Private(AnswerWindow * p):self(p) 
    {;}

    AnswerWindow * self;
    QLineEdit * lineEdit;
};

/*!
 * Creates the object.
 */
AnswerWindow::AnswerWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private(this);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 600) / 800;
    int h = (height*  50) / 600;
    resize(w,h);
    
    int hideY = height;
    int showY = (height * 400) / 600;
    int x     = (width * 100) / 800;

    setMovingEffect(QPoint(x, hideY), QPoint(x, showY));
    move(x, hideY);

    setContentsMargins(h/2, h/4, h/2, h/4);

    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (h/2 * 60 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    QLabel * l_answer = new QLabel(tr("Answer:"), this);
    l_answer->setFixedHeight(h/2);
    l_answer->setFont(font);

    {
        QPalette pal = palette();
        pal.setBrush( QPalette::Foreground, QBrush(QColor("#888")) );
        l_answer->setPalette( pal );
    }
    
    d->lineEdit = new QLineEdit(this);
    d->lineEdit->setFixedHeight(h/2);
    d->lineEdit->setFont(font);
    
    QHBoxLayout * hbox = new QHBoxLayout;
    hbox->addWidget(l_answer);
    hbox->addWidget(d->lineEdit);
    hbox->setSpacing(h/4);
    hbox->setMargin(0);
    setLayout(hbox);

}

/*!
 * Deletes it.
 */
AnswerWindow::~AnswerWindow()
{
    delete d;
}

void AnswerWindow::showWindow()
{
    moveForward();
}

void AnswerWindow::hideWindow()
{
    moveBackward();
}


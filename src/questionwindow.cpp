// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtGui>
#include "questionwindow.h"

#define SCROLL_MSECS 200

class QuestionWindow::Private {
public:
    Private(QuestionWindow * p):self(p) 
    {;}

    QuestionWindow * self;
    QLabel * textEdit;
};

/*!
 * Creates the object.
 */
QuestionWindow::QuestionWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private(this);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 600) / 800;
    int h = (height* 230) / 600;
    resize(w,h);
    
    int hideY = -h;
    int showY = (height * 50) / 600;
    int x     = (width * 100) / 800;
    setMovingEffect(QPoint(x, hideY), QPoint(x, showY));
    move(x, hideY);

    int b = (height*  25) / 600;
    setContentsMargins(b, b, b, b);

    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (b * 60 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    QLabel * l_question = new QLabel(tr("Question:"), this);
    l_question->setFixedHeight(b);
    l_question->setFont(font);
    
    d->textEdit = new QLabel(this);

    {
        //d->textEdit->setReadOnly(true);
        d->textEdit->setFrameStyle(QFrame::NoFrame);
        d->textEdit->setLineWidth(0);
        d->textEdit->setAlignment(Qt::AlignCenter);
        d->textEdit->setText("Question...");

        QPalette pal = palette();
        pal.setBrush( QPalette::Base, QBrush(QColor("#ffffd0")) );
        d->textEdit->setPalette( pal );

        pal.setBrush( QPalette::Foreground, QBrush(QColor("#888")) );
        l_question->setPalette( pal );

        QFont teFont = font;
        teFont.setPointSizeF(font.pointSizeF()*1.5);
        d->textEdit->setFont(teFont);
    }
    
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(l_question,  0,0);
    grid->addWidget(d->textEdit, 1,0);
    grid->setSpacing(0);
    grid->setMargin(0);
    setLayout(grid);
}

/*!
 * Deletes it.
 */
QuestionWindow::~QuestionWindow()
{
    delete d;
}

void QuestionWindow::showWindow()
{
    moveForward();
}

void QuestionWindow::hideWindow()
{
    moveBackward();
}


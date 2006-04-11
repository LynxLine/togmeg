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
    QLabel * l_header;
    QLabel * l_question;
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
    
    d->l_header = new QLabel(tr("Question:"), this);
    d->l_header->setFixedHeight(b);
    d->l_header->setFont(font);
    
    d->l_question = new QLabel(this);

    {
        d->l_question->setFrameStyle(QFrame::NoFrame);
        d->l_question->setLineWidth(0);
        d->l_question->setAlignment(Qt::AlignCenter);

        QPalette pal = palette();
        pal.setBrush( QPalette::Base, QBrush(QColor("#ffffd0")) );
        d->l_question->setPalette( pal );

        pal.setBrush( QPalette::Foreground, QBrush(QColor("#888")) );
        d->l_header->setPalette( pal );

        QFont teFont = font;
        teFont.setPointSizeF(font.pointSizeF()*1.5);
        d->l_question->setFont(teFont);
    }
    
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(d->l_header,  0,0);
    grid->addWidget(d->l_question, 1,0);
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

void QuestionWindow::setQuestion(QString question)
{
    d->l_question->setText(question);
}

void QuestionWindow::setHeader(QString header)
{
    d->l_header->setText(header);
}


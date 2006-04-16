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

    QLabel * l_answer;
    QLineEdit * le_answer;
    QString answer;
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
    
    d->l_answer = new QLabel(tr("Answer:"), this);
    d->l_answer->setFixedHeight(h/2);
    d->l_answer->setFont(font);

    {
        QPalette pal = palette();
        pal.setBrush( QPalette::Foreground, QBrush(QColor("#888")) );
        d->l_answer->setPalette( pal );
    }
    
    d->le_answer = new QLineEdit(this);
    d->le_answer->setFixedHeight(h/2);
    d->le_answer->setFont(font);
    
    setFocusProxy( d->le_answer );
    
    connect(d->le_answer, SIGNAL(returnPressed()),
            this,        SIGNAL(returnPressed()));
    
    QHBoxLayout * hbox = new QHBoxLayout;
    hbox->addWidget(d->l_answer);
    hbox->addWidget(d->le_answer);
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

void AnswerWindow::setAnswer(QString answer)
{
    d->answer = answer;
}

QString AnswerWindow::answer()
{
    return d->answer;
}

bool AnswerWindow::isAnswerCorrect()
{
    return d->le_answer->text() == d->answer;
}

void AnswerWindow::clear()
{
    d->le_answer->clear();
}

void AnswerWindow::setState(State state)
{
    QPalette pal = d->le_answer->palette();
    if      (state == AnswerWindow::Default) pal.setBrush( QPalette::Base, QBrush(QColor("#fff")) );
    else if (state == AnswerWindow::Mistake) pal.setBrush( QPalette::Base, QBrush(QColor("#faa")) );
    
    d->le_answer->setPalette( pal );
}


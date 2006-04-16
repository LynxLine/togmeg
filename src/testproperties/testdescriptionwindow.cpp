// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtSql>
#include "testdescriptionwindow.h"

class TestDescriptionWindow::Private {
public:
    QLabel * l_header;
    QListWidget * lw_repository;

    bool init;
};

/*!
 * Creates the object.
 */
TestDescriptionWindow::TestDescriptionWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private;
    d->init = false;
    setStyle(Round10);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 385) / 800;
    int h = (height* 250) / 600;
    resize(w,h);
    
    int hideX = -w;
    int y         = (height * 50) / 600;
    int showX     = (width *  10) / 800;
    setMovingEffect(QPoint(hideX, y), QPoint(showX, y));
    move(hideX, y);

    int b = (height*  10) / 600;
    
    setContentsMargins(1, b, 1, b);

    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (b * 60 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    /*
    d->l_header = new QLabel(tr("Test Repository:"), this);
    d->l_header->setFixedHeight(b);
    d->l_header->setFont(font);
    */
    
    d->lw_repository = new QListWidget(this);

    {
        d->lw_repository->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        d->lw_repository->setLineWidth(1);
        
        d->lw_repository->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        d->lw_repository->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        QPalette pal = palette();
        pal.setBrush( QPalette::Base, QBrush(QColor("#ffffd0")) );
        pal.setBrush( QPalette::Window, QBrush(QColor("#f0f0d0")) );
        d->lw_repository->setPalette( pal );

        //pal.setBrush( QPalette::Foreground, QBrush(QColor("#888")) );
        //d->l_header->setPalette( pal );

        QFont teFont = font;
        teFont.setPointSizeF(font.pointSizeF()*1.2);
        d->lw_repository->setFont(teFont);
    }

    setFocusProxy( d->lw_repository );
    
    QGridLayout * grid = new QGridLayout;
    //grid->addWidget(d->l_header,  0,0);
    grid->addWidget(d->lw_repository, 0,0);
    grid->setSpacing(5);
    grid->setMargin(0);
    setLayout(grid);
}

/*!
 * Deletes it.
 */
TestDescriptionWindow::~TestDescriptionWindow()
{
    delete d;
}

void TestDescriptionWindow::showWindow()
{
    moveForward();
    QTimer::singleShot(250, this, SLOT(loadData()));
}

void TestDescriptionWindow::hideWindow()
{
    moveBackward();
}

void TestDescriptionWindow::loadData()
{
    if ( !d->init ) {
        QSqlQuery createQuery;
        createQuery.exec("create table repository( "\
                   " id          integer primary key autoincrement, "\
                   " name        varchar(400) "\
                   ")");
        d->init = true;
    }
    
    d->lw_repository->clear();
    QSqlQuery query("SELECT id, name FROM repository");
    while (query.next()) {
        QString name = query.value(1).toString();
        qDebug() << name;
        d->lw_repository->addItem(name);
    }
}

void TestDescriptionWindow::newTest()
{
    QSqlQuery query;
    query.prepare("INSERT INTO repository (name) VALUES (:name)");
    query.bindValue(":name", "<name>");
    query.exec();

    loadData();
}

void TestDescriptionWindow::keyPressEvent(QKeyEvent * ke)
{
    if (ke->key() == Qt::Key_Escape) {
        ke->ignore();
        return;
    }
    
    if (ke->key() == Qt::Key_Insert) {
        ke->accept();
        newTest();
        return;
    }
    
    BoxWindow::keyPressEvent(ke);
}


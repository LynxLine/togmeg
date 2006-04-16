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
    
    setContentsMargins(5, 5, 5, 5);

    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (b * 60 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    d->l_header = new QLabel(tr("Test Repository:"), this);
    d->l_header->setFixedHeight(b);
    d->l_header->setFont(font);
    
    d->lw_repository = new QListWidget(this);

    {
        d->lw_repository->setFrameStyle(QFrame::NoFrame);
        d->lw_repository->setLineWidth(0);
        
        d->lw_repository->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        d->lw_repository->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        QPalette pal = palette();
        pal.setBrush( QPalette::Base, QBrush(QColor("#ffffd0")) );
        pal.setBrush( QPalette::Window, QBrush(QColor("#f0f0d0")) );
        d->lw_repository->setPalette( pal );

        pal.setBrush( QPalette::Window, QBrush(QColor("#ff7030")) );
        pal.setBrush( QPalette::Foreground, QBrush(QColor("#ffffff")) );
        d->l_header->setAutoFillBackground(true);
        d->l_header->setPalette( pal );

        QFont teFont = font;
        teFont.setPointSizeF(font.pointSizeF()*1.2);
        d->lw_repository->setFont(teFont);
    }

    QHBoxLayout * hbox = new QHBoxLayout;

    QToolButton * tb_add = new QToolButton;
    QToolButton * tb_mod = new QToolButton;
    QToolButton * tb_del = new QToolButton;

    tb_add->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tb_mod->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tb_del->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    tb_add->setAutoRaise(true);
    tb_mod->setAutoRaise(true);
    tb_del->setAutoRaise(true);

    tb_add->setText(tr("Add"));
    tb_mod->setText(tr("Edit"));
    tb_del->setText(tr("Delete"));

    tb_add->setIcon( QPixmap(":/images/actions/add.png") );
    tb_mod->setIcon( QPixmap(":/images/actions/edit.png") );
    tb_del->setIcon( QPixmap(":/images/actions/remove.png") );

    connect(tb_add, SIGNAL(clicked()),
            this, SLOT(addTest()));
    connect(tb_mod, SIGNAL(clicked()),
            this, SLOT(editTest()));
    connect(tb_del, SIGNAL(clicked()),
            this, SLOT(removeTest()));

    {
        QFont font( "Verdana", 9, QFont::Bold );
        font.setStyleStrategy(QFont::PreferAntialias);
        tb_add->setFont(font);
        tb_mod->setFont(font);
        tb_del->setFont(font);
    }
    
    hbox->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    hbox->addWidget(tb_add);
    hbox->addWidget(tb_mod);
    hbox->addWidget(tb_del);
    hbox->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    
    hbox->setSpacing(0);
    hbox->setMargin(2);
    
    QFrame * hline;
    hline = new QFrame;
    hline->setFrameStyle(QFrame::HLine | QFrame::Plain);
    {
        QPalette pal = palette();
        pal.setBrush( foregroundRole(), QBrush(QColor("#e0e0e0")) );
        hline->setPalette( pal );
    }
    
    setFocusProxy( d->lw_repository );
    
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(d->l_header,      0,0);
    grid->addWidget(d->lw_repository, 1,0);
    grid->addWidget(hline,            2,0);
    grid->addLayout(hbox,             3,0);
    grid->setSpacing(0);
    grid->setMargin(0);
    setLayout(grid);

    connect(d->lw_repository->itemDelegate(), SIGNAL(commitData(QWidget *)),
            this, SLOT(updateTestName()));
    connect(d->lw_repository, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(currentItemChanged(QListWidgetItem *)));
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
    if ( !d->init ) QTimer::singleShot(250, this, SLOT(loadData()));
}

void TestDescriptionWindow::hideWindow()
{
    moveBackward();
}

#define COL_REP_ID    0
#define COL_REP_NAME  1

void TestDescriptionWindow::loadData(int selectID, bool edit)
{
    if ( !d->init ) {
        QSqlQuery createQuery;
        createQuery.exec("create table repository( "\
                   " id          integer primary key autoincrement, "\
                   " name        varchar(400), "\
                   " active      bool "\
                   ")");
        d->init = true;
    }
    
    QListWidgetItem * selectedItem = 0L;
    d->lw_repository->clear();
    QSqlQuery query("SELECT id, name FROM repository");
    while (query.next()) {
        int     id   = query.value(COL_REP_ID).toInt();
        QString name = query.value(COL_REP_NAME).toString();
        QListWidgetItem * item = new QListWidgetItem(name, d->lw_repository);
        item->setData(Qt::UserRole, query.value(COL_REP_ID));
        item->setFlags(item->flags() | Qt::ItemIsEditable);

        if ( selectID == id) {
            d->lw_repository->setCurrentItem(item);
            selectedItem = item;
        }
    }

    if (edit && selectedItem) {
        d->lw_repository->editItem(selectedItem);
    }
}

void TestDescriptionWindow::newTest()
{
    QSqlQuery query;
    query.prepare("INSERT INTO repository (name) VALUES (:name)");
    query.bindValue(":name", "");
    query.exec();

    loadData(query.lastInsertId().toInt(), true);
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

    if (ke->key() == Qt::Key_F2) {
        ke->accept();
        if ( d->lw_repository->currentItem() )
            d->lw_repository->editItem( d->lw_repository->currentItem() );
        return;
    }
    
    BoxWindow::keyPressEvent(ke);
}

void TestDescriptionWindow::updateTestName()
{
    if ( !d->lw_repository->currentItem() ) return;
    
    QString newName = d->lw_repository->currentItem()->text();
    int id = d->lw_repository->currentItem()->data(Qt::UserRole).toInt();

    QSqlQuery query;
    query.prepare(QString("UPDATE repository SET name = :name WHERE id = '%1'").arg(id));
    query.bindValue(":name", newName);
    query.exec();
}

void TestDescriptionWindow::currentItemChanged(QListWidgetItem * current)
{
    if ( !current ) return;
    int id = current->data(Qt::UserRole).toInt();
    emit testSelected(id);
    setFocus();
}


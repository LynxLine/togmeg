// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtSql>
#include "testeditorwindow.h"

class TestEditorWindow::Private {
public:
    QLabel * l_header;
    QTableView * tv_editor;

    QPointer<QSqlTableModel> model;
};

/*!
 * Creates the object.
 */
TestEditorWindow::TestEditorWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private;
    setStyle(Round10);

    QDesktopWidget *desktop = QApplication::desktop();
    int height = desktop->height();
    int width = desktop->width();

    int w = (width * 385) / 800;
    int h = (height* 500) / 600;
    resize(w,h);
    
    int hideX = width;
    int y         = (height * 50) / 600;
    int showX     = (width * 405) / 800;
    setMovingEffect(QPoint(hideX, y), QPoint(showX, y));
    move(hideX, y);

    int b = (height*  10) / 600;

    QFont font("Verdana", 10);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);
    
    int pixelSize = (b * 60 )/100;
    float pointSize = ( font.pointSizeF() * pixelSize )/ 10;
    font.setPointSizeF(pointSize);
    
    d->l_header = new QLabel(tr("Questions/Answers Editor:"), this);
    d->l_header->setFixedHeight(b);
    d->l_header->setFont(font);
    
    d->tv_editor = new QTableView(this);

    {
        //d->tv_editor->setFrameStyle(QFrame::NoFrame);
        //d->tv_editor->setLineWidth(0);
        //

        d->tv_editor->verticalHeader()->hide();
        
        QPalette pal = palette();
        pal.setBrush( QPalette::Base, QBrush(QColor("#ffffd0")) );
        pal.setBrush( QPalette::Window, QBrush(QColor("#f0f0d0")) );
        d->tv_editor->setPalette( pal );

        pal.setBrush( QPalette::Foreground, QBrush(QColor("#888")) );
        d->l_header->setPalette( pal );

        /*
        QFont teFont = font;
        teFont.setPointSizeF(font.pointSizeF()*1.5);
        d->tw_edior->setFont(teFont);
        */
    }
    
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(d->l_header,  0,0);
    grid->addWidget(d->tv_editor, 1,0);
    grid->setSpacing(5);
    grid->setMargin(0);
    setLayout(grid);

}

/*!
 * Deletes it.
 */
TestEditorWindow::~TestEditorWindow()
{
    delete d;
}

void TestEditorWindow::showWindow()
{
    moveForward();
    QTimer::singleShot(250, this, SLOT(loadData()));
}

void TestEditorWindow::hideWindow()
{
    moveBackward();
}

void TestEditorWindow::loadData()
{
    if ( d->model ) return;
    
    d->model = new QSqlTableModel(this);
    
    d->model->setTable("test");
    d->model->setEditStrategy(QSqlTableModel::OnFieldChange);
    d->model->select();
    
    d->model->setHeaderData(1, Qt::Horizontal, tr("Question"));
    d->model->setHeaderData(2, Qt::Horizontal, tr("Answer"));

    d->tv_editor->setModel(d->model);
    d->tv_editor->setColumnHidden(0, true);

    d->tv_editor->setColumnWidth(1, (width()-25)/2 );
    d->tv_editor->setColumnWidth(2, (width()-25)/2 );
}

void TestEditorWindow::keyPressEvent(QKeyEvent * ke)
{
    if (ke->key() == Qt::Key_Insert) {
        d->model->insertRow( d->model->rowCount() );
    }
    
    BoxWindow::keyPressEvent(ke);
}


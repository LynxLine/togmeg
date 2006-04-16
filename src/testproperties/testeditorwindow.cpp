// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtSql>
#include "testeditorwindow.h"

#define COL_ID          0
#define COL_TID         1
#define COL_QUESTION    2
#define COL_ANSWER      3
#define COL_MSECS       4
#define ROW_APPEND      -1

class TestEditorWindow::Private {
public:
    QLabel * l_header;
    QTableView * tv_editor;

    QPointer<QSqlTableModel> model;

    int testID;
    int insertRow;
    QSqlRecord insertRecord;
};

/*!
 * Creates the object.
 */
TestEditorWindow::TestEditorWindow(QWidget * parent)
:BoxWindow(parent)
{
    d = new Private;

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

    setContentsMargins(5, 5, 5, 5);

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
        d->tv_editor->setFrameStyle(QFrame::NoFrame);
        d->tv_editor->setLineWidth(0);
        
        d->tv_editor->verticalHeader()->hide();
        d->tv_editor->horizontalHeader()->hide();
        d->tv_editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        d->tv_editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        QPalette pal = palette();
        pal.setBrush( QPalette::Base, QBrush(QColor("#ffffd0")) );
        pal.setBrush( QPalette::Window, QBrush(QColor("#f0f0d0")) );
        d->tv_editor->setPalette( pal );

        pal.setBrush( QPalette::Window, QBrush(QColor("#ff7030")) );
        pal.setBrush( QPalette::Foreground, QBrush(QColor("#ffffff")) );
        d->l_header->setAutoFillBackground(true);
        d->l_header->setPalette( pal );

        QFont teFont = font;
        teFont.setPointSizeF(font.pointSizeF()*1.2);
        d->tv_editor->setFont(teFont);
    }

    setFocusProxy( d->tv_editor );
    
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(d->l_header,  0,0);
    grid->addWidget(d->tv_editor, 1,0);
    grid->setSpacing(0);
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
    //QTimer::singleShot(250, this, SLOT(loadData()));
}

void TestEditorWindow::hideWindow()
{
    moveBackward();
}

void TestEditorWindow::loadData(int testID)
{
    if ( d->model )  {
        submitAll();
        delete d->model;
    }
    
    d->testID = testID;
    d->model = new QSqlTableModel(this);
    
    d->model->setTable("test");
    d->model->setFilter(QString("tid='%1'").arg(testID));
    d->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    d->model->select();
    
    d->model->setHeaderData(COL_QUESTION, Qt::Horizontal, tr("Question"));
    d->model->setHeaderData(COL_ANSWER,   Qt::Horizontal, tr("Answer"));

    d->tv_editor->setModel(d->model);
    d->tv_editor->setColumnHidden(COL_ID, true);
    d->tv_editor->setColumnHidden(COL_TID, true);
    d->tv_editor->setColumnHidden(COL_MSECS, true);
    d->tv_editor->setSelectionMode(QAbstractItemView::NoSelection);
    
    int vScrollWidth = d->tv_editor->verticalScrollBar()->width();
    d->tv_editor->setColumnWidth(COL_QUESTION, (width()-vScrollWidth -10)/2 );
    d->tv_editor->setColumnWidth(COL_ANSWER,   (width()-vScrollWidth -10)/2 );
    
    d->tv_editor->setEditTriggers(QAbstractItemView::AllEditTriggers);
    
    connect(d->model, SIGNAL(primeInsert(int, QSqlRecord &)),
            this,       SLOT(primeInsert(int, QSqlRecord &)));
    connect(d->tv_editor->itemDelegate(), SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)));

    if (d->model->rowCount()) {
        QModelIndex index = d->model->index(0, COL_QUESTION);
        d->tv_editor->setCurrentIndex(index);
    }
}

void TestEditorWindow::keyPressEvent(QKeyEvent * ke)
{
    if (ke->key() == Qt::Key_Escape) {
        ke->ignore();
        return;
    }
    
    if (ke->key() == Qt::Key_Insert) {
        ke->accept();
        newRow();
        return;
    }
    
    BoxWindow::keyPressEvent(ke);
}

void TestEditorWindow::primeInsert(int row, QSqlRecord & record)
{
    d->insertRow = row;
    d->insertRecord = record;
    QTimer::singleShot(10, this, SLOT(primeInsertContinue()));
}
    
void TestEditorWindow::primeInsertContinue()
{
    QModelIndex index = d->model->index(d->insertRow, COL_QUESTION);
    QString text = d->model->data(index).toString();
    
    d->tv_editor->scrollTo(index, QAbstractItemView::PositionAtBottom);
    d->tv_editor->setCurrentIndex(index);
}

void TestEditorWindow::newRow()
{
    if ( !d->model ) return;
        
    QSqlRecord newRecord( d->model->record() );
    
    newRecord.setGenerated("id", true);
    newRecord.setValue("tid", d->testID);
    newRecord.setValue("question", "");
    newRecord.setValue("answer", "");
    newRecord.setValue("msecs", 10* 1000);

    bool ok = d->model->insertRecord( -1,  newRecord);

    if (!ok) {
       qDebug() << d->model->lastError().text();
       qDebug() << d->model->query().lastQuery();
       return;
    } 
}

void TestEditorWindow::closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint hint)
{
    if (hint == QAbstractItemDelegate::NoHint ||
        hint == QAbstractItemDelegate::RevertModelCache) return;
    QTimer::singleShot(10, this, SLOT(closeEditorContinue()));
}
    
void TestEditorWindow::closeEditorContinue()
{
    QModelIndex index = d->tv_editor->currentIndex();
    QModelIndex answerIndex = d->model->index(index.row(), COL_ANSWER );
    QModelIndex questionIndex = d->model->index(index.row(), COL_QUESTION );
    
    QString answer = d->model->data(answerIndex).toString();
    QString question = d->model->data(questionIndex).toString();
    
    if (index.column() == COL_QUESTION) {
        index = d->model->index(index.row(), COL_ANSWER );
        d->tv_editor->setCurrentIndex(index);
        
        return;
    }
    else if (index.column() == COL_ANSWER) {
        
        if (index.row() == d->model->rowCount()-1) {
            if ( !question.isEmpty() && !answer.isEmpty() ) {
                newRow();
                return;
            }

            if (question.isEmpty()) d->tv_editor->setCurrentIndex(questionIndex);
            else if (answer.isEmpty()) {
                d->tv_editor->setCurrentIndex(answerIndex);
                d->tv_editor->edit(index);
            }
        }
        else {
            index = d->model->index(index.row()+1, COL_QUESTION );
            d->tv_editor->setCurrentIndex(index);
        }
    }
}

void TestEditorWindow::submitAll()
{
    if ( !d->model ) return;
        
    for (int row=0;row < d->model->rowCount();row++) {
        QSqlRecord record = d->model->record(row);

        QString answer = record.value("answer").toString();
        QString question = record.value("question").toString();

        if (answer.isEmpty() && question.isEmpty()) {
            d->model->removeRows(row, 1);
        }
    }
    
    d->model->submitAll();
}


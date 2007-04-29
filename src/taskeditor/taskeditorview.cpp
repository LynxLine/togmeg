//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "mainwindow.h"
#include "headerview.h"
#include "taskeditorview.h"
#include "studytaskmodel.h"

class TaskEditorView::Private
{
public:
    QPointer<StudyTaskModel> model;
    QPointer<QMenu> contextMenu;
    QPointer<HeaderView> header;

    QColor c1;
    QColor c2;
};

TaskEditorView::TaskEditorView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    d->header = new HeaderView(Qt::Horizontal, this);
    d->header->setFont( MainWindow::baseFont(0.95) );
    d->header->setStretchLastSection(true);

    setHeader( d->header );
    setRootIsDecorated(false);
    setAutoFillBackground(true);

#ifdef Q_WS_MAC
    setAlternatingRowColors(false);
    {
        d->c1 = palette().color(QPalette::Base);
        d->c2 = palette().color(QPalette::AlternateBase);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, QColor(0,0,0,0));
        setPalette(palette);
    }
#else 
    setAlternatingRowColors(true);
#endif

    setFrameStyle(QFrame::NoFrame);
    setAllColumnsShowFocus(true);
    setItemDelegate(new TaskEditorItemDelegate(this));
    setEditTriggers(
            QAbstractItemView::DoubleClicked |
            QAbstractItemView::SelectedClicked |
            QAbstractItemView::EditKeyPressed |
            QAbstractItemView::AnyKeyPressed
        );

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Add new Question") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("&Remove"), this, SLOT(removeEntry()));

    d->model = new StudyTaskModel(this);
    setModel( d->model );

    header()->setDefaultAlignment(Qt::AlignLeft);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::Stretch);
    header()->setResizeMode(2, QHeaderView::Stretch);
}

TaskEditorView::~TaskEditorView()
{
    qDebug() << "~TaskEditorView";
    delete d;
}

void TaskEditorView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void TaskEditorView::openTask(QString taskId)
{
    d->model->load(taskId);
    setFocus();
    if ( d->model->rowCount() )
        setCurrentIndex( d->model->index(0,0) );
}

void TaskEditorView::addNewEntry()
{
    QModelIndex index = d->model->addNewEntry();
    setCurrentIndex( index );
    edit( index );
}

void TaskEditorView::removeEntry()
{
    QModelIndex index = currentIndex();
    d->model->removeEntry( index );
    scrollTo(currentIndex());
}

void TaskEditorView::paintEvent(QPaintEvent * pe)
{
#ifdef Q_WS_MAC
    QRect r = pe->rect();
    QPainter p( viewport() );

    int rowHeight = 20;
    int i1 = r.y()/rowHeight-1;
    int i2 = (r.y()+r.height())/rowHeight+1;

    for (int i=i1;i<i2;i++) {
        QRect rf = QRect(r.x(), i*rowHeight, r.width(), rowHeight);
        p.fillRect(rf, i % 2 ? d->c2 : d->c1);
    }

    p.end();
#endif

    QTreeView::paintEvent(pe);
}

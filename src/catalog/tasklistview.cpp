//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "tasklistview.h"

class TaskListView::Private
{
public:
    QPointer<QMenu> contextMenu;
};

TaskListView::TaskListView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;

    setAutoFillBackground(true);
    setAlternatingRowColors(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Create new Study") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("Remove") );

    QStringList test;
    test << "Test1" << "test2";

    QStringListModel * model = new QStringListModel(test, this);
    setModel(model);
}

TaskListView::~TaskListView()
{
    qDebug() << "~TaskListView";
    delete d;
}

void TaskListView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( mapToGlobal(pos) );
}

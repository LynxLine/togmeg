//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "categoryview.h"
#include "categorymodel.h"
#include "catalogwidget.h"
#include "tasklistview.h"
#include "pixmapbutton.h"

#include "mainwindow.h"

class CatalogWidget::Private
{
public:
    CategoryView * categoryView;
    TaskListView * taskListView;
    CategoryModel * categoryModel;
};

/*!
 Crestes CatalogWidget
 */
CatalogWidget::CatalogWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QGridLayout * layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    //category area
    QVBoxLayout * categoryLayout = new QVBoxLayout;
    categoryLayout->setMargin(0);
    categoryLayout->setSpacing(5);
    layout->addLayout(categoryLayout, 0, 0);

    d->categoryModel = new CategoryModel(this);
    d->categoryView = new CategoryView(this);

    {
        d->categoryView->setFixedWidth(200);
        d->categoryView->setModel( d->categoryModel );
        d->categoryView->setCurrentIndex( d->categoryModel->index(0,0) );
    }

    categoryLayout->addWidget( d->categoryView );

    //task list area
    QVBoxLayout * taskLayout = new QVBoxLayout;
    taskLayout->setMargin(0);
    taskLayout->setSpacing(0);
    layout->addLayout(taskLayout, 0, 2, 3,1);

    d->taskListView = new TaskListView(this);
    taskLayout->addWidget( d->taskListView );

    QFrame * vline = new QFrame;
    {
        QPalette palette = vline->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        vline->setPalette(palette);
        vline->setFrameStyle(QFrame::VLine | QFrame::Plain);
        vline->setFixedWidth(1);
        vline->setLineWidth(1);
    }
    layout->addWidget( vline, 0, 1, 3,1);

    QFrame * hline = new QFrame;
    {
        QPalette palette = hline->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        hline->setPalette(palette);
        hline->setFrameStyle(QFrame::HLine | QFrame::Plain);
        hline->setFixedHeight(1);
        hline->setLineWidth(1);
    }
    layout->addWidget( hline, 1, 0 );

    //bottom footer
    QHBoxLayout * footerAreaLayout = new QHBoxLayout;
    footerAreaLayout->setMargin(0);
    footerAreaLayout->setSpacing(0);
    layout->addLayout( footerAreaLayout, 2, 0);

    PixmapButton * addCategory = new PixmapButton(
            ":/images/icons/add-button-30x22.png",
            ":/images/icons/add-button-30x22.png",
            ":/images/icons/add-button-pressed-30x22.png"
        );
    PixmapButton * remCategory = new PixmapButton(
            ":/images/icons/remove-button-30x22.png",
            ":/images/icons/remove-button-30x22.png",
            ":/images/icons/remove-button-pressed-30x22.png"
        );

    QFrame * vline2 = new QFrame;
    {
        QPalette palette = vline2->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        vline2->setPalette(palette);
        vline2->setFrameStyle(QFrame::VLine | QFrame::Plain);
        vline2->setFixedWidth(1);
        vline2->setLineWidth(1);
    }

    QFrame * vline3 = new QFrame;
    {
        QPalette palette = vline3->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        vline3->setPalette(palette);
        vline3->setFrameStyle(QFrame::VLine | QFrame::Plain);
        vline3->setFixedWidth(1);
        vline3->setLineWidth(1);
    }

    //addCategory->setText("+");
    //remCategory->setText("-");

    //addCategory->setAutoRaise(true);
    //remCategory->setAutoRaise(true);

    //addCategory->setFixedSize(25,25);
    //remCategory->setFixedSize(25,25);

    //addCategory->setFocusPolicy(Qt::NoFocus);
    //remCategory->setFocusPolicy(Qt::NoFocus);

    connect(addCategory, SIGNAL(clicked()), d->categoryView, SLOT(addNewCategory()));
    connect(remCategory, SIGNAL(clicked()), d->categoryView, SLOT(removeCategory()));

    footerAreaLayout->addWidget( addCategory );
    footerAreaLayout->addWidget( vline2 );
    footerAreaLayout->addWidget( remCategory );
    footerAreaLayout->addWidget( vline3 );

    QWidget * space = new QWidget;
    {
        space->setFocusPolicy(Qt::NoFocus);
        QPalette palette = space->palette();
        palette.setBrush(QPalette::Window, QPixmap(":/images/icons/bg-bottom-category.png"));
        space->setAutoFillBackground(true);
        space->setPalette(palette);
    }
    footerAreaLayout->addWidget(space);

    connect(d->categoryView, SIGNAL(categoryActivated(QString)),
            d->taskListView, SLOT(applyCategoryFilter(QString)));
    connect(d->taskListView, SIGNAL(studyTaskActivated(QString)),
            this,            SIGNAL(studyTaskActivated(QString)));
    connect(d->taskListView, SIGNAL(currentTaskChanged(QString)),
            this,            SIGNAL(currentTaskChanged(QString)));

    setFocusProxy( d->taskListView );
    d->categoryView->setFocus();
}

/*!
 Deletes the object.
 */
CatalogWidget::~CatalogWidget()
{
    delete d;
}

QString CatalogWidget::currentTaskId()
{
    return d->taskListView->currentTaskId();
}

void CatalogWidget::addNewStudy()
{
    d->taskListView->addNewStudy();
}

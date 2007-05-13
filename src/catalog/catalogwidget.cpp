//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "categoryview.h"
#include "categorymodel.h"
#include "catalogwidget.h"
#include "catalogfooter.h"
#include "tasklistview.h"
#include "pixmapbutton.h"

#include "mainwindow.h"

class CatalogWidget::Private
{
public:
    CategoryView * categoryView;
    TaskListView * taskListView;
    CatalogFooter * catalogFooter;

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
    layout->addLayout(taskLayout, 0, 2);

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
    layout->addWidget( vline, 0, 1, 2,1);

    QFrame * hline1 = new QFrame;
    {
        QPalette palette = hline1->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        hline1->setPalette(palette);
        hline1->setFrameStyle(QFrame::HLine | QFrame::Plain);
        hline1->setFixedHeight(1);
        hline1->setLineWidth(1);
    }
    layout->addWidget( hline1, 1, 0 );

    QFrame * hline2 = new QFrame;
    {
        QPalette palette = hline2->palette();
        palette.setColor(QPalette::WindowText, "#999999");
        hline2->setPalette(palette);
        hline2->setFrameStyle(QFrame::HLine | QFrame::Plain);
        hline2->setFixedHeight(1);
        hline2->setLineWidth(1);
    }
    layout->addWidget( hline2, 1, 2 );

    //bottom categories footer
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

    d->catalogFooter = new CatalogFooter;
    layout->addWidget( d->catalogFooter, 2,1, 1,2 );

    connect(d->categoryView, SIGNAL(categoryActivated(QString)),
            d->taskListView,   SLOT(applyCategoryFilter(QString)));
    connect(d->taskListView, SIGNAL(studyTaskActivated(QString)),
            this,            SIGNAL(studyTaskActivated(QString)));
    connect(d->taskListView, SIGNAL(currentTaskChanged(QString)),
            this,            SIGNAL(currentTaskChanged(QString)));

    connect(d->taskListView, SIGNAL(currentTaskChanged(QString)),
            d->catalogFooter,  SLOT(setCurrentTask(QString)));
    connect(d->taskListView, SIGNAL(rowCountChanged(int)),
            d->catalogFooter,  SLOT(setStudyCount(int)));

    connect(d->catalogFooter, SIGNAL(renameClicked()),
            d->taskListView,    SLOT(editStudyName()));

    //init footer
    d->catalogFooter->setCurrentTask( d->taskListView->currentTaskId() );
    d->catalogFooter->setStudyCount( d->taskListView->model()->rowCount() );

    //focus
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

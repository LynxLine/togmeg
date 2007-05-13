//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "studytask.h"
#include "mainwindow.h"
#include "pixmapbutton.h"
#include "tasklistmodel.h"
#include "catalogfooter.h"

class CatalogFooter::Private {
public:
    Private() 
    :studyCount(0), currentTaskEntryCount(0)
    {;}
    QLabel * l_message;

    int studyCount;
    QString currentTaskName;
    int currentTaskEntryCount;
};

/*!
 * Creates the object.
 */
CatalogFooter::CatalogFooter(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    setFixedHeight(22);
    setFocusPolicy(Qt::NoFocus);
    setAutoFillBackground(true);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setSpacing(5);
    layout->setMargin(0);
    setLayout(layout);

    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Fixed, QSizePolicy::Minimum));

    PixmapButton * b_rename = new RenamePixmapButton;
    b_rename->setFont( MainWindow::baseFont(0.95) );
    connect(b_rename, SIGNAL(clicked()), this, SIGNAL(renameClicked()));
    layout->addWidget( b_rename );

    d->l_message = new QLabel;
    d->l_message->setAlignment( Qt::AlignCenter );
    d->l_message->setFont( MainWindow::baseFont(0.95) );
    layout->addWidget( d->l_message );

    QPalette palette = this->palette();
    palette.setColor(QPalette::Text, "#505050");
    palette.setColor(QPalette::WindowText, "#505050");
    palette.setBrush(QPalette::Window, QPixmap(":/images/icons/bg-bottom-category.png"));
    setPalette(palette);
}

/*!
 * Deletes it.
 */
CatalogFooter::~CatalogFooter()
{
    delete d;
}

void CatalogFooter::setStudyCount(int count)
{
    d->studyCount = count;
    updateMessageLabel();
}

void CatalogFooter::setCurrentTask(QString taskId)
{
    d->currentTaskName.clear();
    d->currentTaskEntryCount = 0;

    if ( !taskId.isEmpty() ) {
        StudyTask * task = TaskListModel::instance()->task( taskId );
        if (task) {
            d->currentTaskName = task->name();
            d->currentTaskEntryCount = task->entryCount();
        }
    }

    updateMessageLabel();
}

void CatalogFooter::updateMessageLabel()
{
    if ( d->currentTaskName.isEmpty() ) {
        d->l_message->setText(
            tr("Studies: %1")
                .arg( d->studyCount )
        );
    }
    else {
        d->l_message->setText(
            tr("Studies: %1, Current: %2, %3 Questions")
                .arg( d->studyCount )
                .arg( d->currentTaskName )
                .arg( d->currentTaskEntryCount )
        );
    }
}

RenamePixmapButton::RenamePixmapButton(QWidget * parent)
:PixmapButton(":/images/button-rename-study.png",
              ":/images/button-rename-study.png",
              ":/images/button-rename-study-on.png", parent)
{
}

void RenamePixmapButton::paintEvent(QPaintEvent * pe)
{
    PixmapButton::paintEvent(pe);

    QPainter p(this);
    if ( isDown() ) 
        p.setPen( QColor("#303030") );
    else p.setPen( QColor("#505050") );
    p.drawText(rect(), Qt::AlignCenter, tr("Rename"));
}

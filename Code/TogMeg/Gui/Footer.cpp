
#include <QtGui>
#include "TogMegWindow.h"
#include "pixmapbutton.h"
#include "Footer.h"

class TaskEditorFooter::Private {
public:
    Private() 
    :studyCount(0), currentTaskEntryCount(0)
    {;}
    QLabel * l_message;
    PixmapButton * b_qaqaMode;
    PixmapButton * b_qqaaMode;
    TaskEditorView::NextItemMode nextItemMode;

    int studyCount;
    QString currentTaskName;
    int currentTaskEntryCount;
};

/*!
 * Creates the object.
 */
TaskEditorFooter::TaskEditorFooter(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    d->nextItemMode = TaskEditorView::NextItemMode(-1);//undefined

    setFixedHeight(22);
    setFocusPolicy(Qt::NoFocus);
    setAutoFillBackground(true);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);

    d->b_qqaaMode = new PixmapButton(
        QPixmap(":/images/icons/qqaa-order-button-on.png"),
        QPixmap(":/images/icons/qqaa-order-button-on.png"),
        QPixmap(":/images/icons/qqaa-order-button-down.png")
    );
    
    d->b_qaqaMode = new PixmapButton(
        QPixmap(":/images/icons/qaqa-order-button.png"),
        QPixmap(":/images/icons/qaqa-order-button.png"),
        QPixmap(":/images/icons/qaqa-order-button-down.png")
    );

    layout->addItem(new QSpacerItem(5,10, QSizePolicy::Fixed, QSizePolicy::Minimum));
    layout->addWidget( d->b_qqaaMode );
    layout->addWidget( d->b_qaqaMode );
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));

    connect(d->b_qaqaMode, SIGNAL(pressed()), this, SLOT(toQAQAMode()));
    connect(d->b_qqaaMode, SIGNAL(pressed()), this, SLOT(toQQAAMode()));

    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, "#505050");
    palette.setBrush(QPalette::Window, QPixmap(":/images/icons/bg-bottom-category.png"));
    setPalette(palette);
}

/*!
 * Deletes it.
 */
TaskEditorFooter::~TaskEditorFooter()
{
    delete d;
}

void TaskEditorFooter::toQAQAMode()
{
    setNextItemMode(TaskEditorView::QAQAMode);
}

void TaskEditorFooter::toQQAAMode()
{
    setNextItemMode(TaskEditorView::QQAAMode);
}

void TaskEditorFooter::setNextItemMode(TaskEditorView::NextItemMode m)
{
    if ( d->nextItemMode == m ) return;

    if ( m == TaskEditorView::QAQAMode ) {
        d->b_qaqaMode->setPixmap  ( QPixmap(":/images/icons/qaqa-order-button-on.png") );
        d->b_qaqaMode->setPixmapHl( QPixmap(":/images/icons/qaqa-order-button-on.png") );
        d->b_qqaaMode->setPixmap  ( QPixmap(":/images/icons/qqaa-order-button.png") );
        d->b_qqaaMode->setPixmapHl( QPixmap(":/images/icons/qqaa-order-button.png") );
    }
    else if ( m == TaskEditorView::QQAAMode ) {
        d->b_qaqaMode->setPixmap  ( QPixmap(":/images/icons/qaqa-order-button.png") );
        d->b_qaqaMode->setPixmapHl( QPixmap(":/images/icons/qaqa-order-button.png") );
        d->b_qqaaMode->setPixmap  ( QPixmap(":/images/icons/qqaa-order-button-on.png") );
        d->b_qqaaMode->setPixmapHl( QPixmap(":/images/icons/qqaa-order-button-on.png") );
    }

    d->b_qaqaMode->update();
    d->b_qqaaMode->update();

    d->nextItemMode = m;
    emit nextItemModeChanged(m);
}

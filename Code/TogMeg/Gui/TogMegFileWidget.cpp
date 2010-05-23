
#include <QtGui>
#include "TogMegWindow.h"
#include "pixmapbutton.h"
#include "gradientwidget.h"
#include "TogMegFileEdit.h"
#include "TogMegFileWidget.h"

class TaskEditorWidget::Private
{
public:
    TagMegFileEdit * taskEditorView;
};

/*!
 Crestes TaskEditorWidget
 */
TaskEditorWidget::TaskEditorWidget(TogMegFileModel * model, QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    QGridLayout * layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    d->taskEditorView = new TagMegFileEdit(model, this);
    layout->addWidget( d->taskEditorView );


    setFocusProxy( d->taskEditorView );
}

TagMegFileEdit * TaskEditorWidget::view() const
{
    return d->taskEditorView;
}

/*!
 Deletes the object.
 */
TaskEditorWidget::~TaskEditorWidget()
{
    delete d;
}

void TaskEditorWidget::addNewEntry()
{
    d->taskEditorView->addNewEntry();
}

void TaskEditorWidget::setNextItemMode(TagMegFileEdit::NextItemMode m)
{
    d->taskEditorView->setNextItemMode(m);
}


#ifndef TASKEDITORWIDGET_H
#define TASKEDITORWIDGET_H

#include <QWidget>
#include "TogMegFileEdit.h"

class TagMegFileEdit;
class TogMegFileModel;

/*!
 */
class TaskEditorWidget : public QWidget
{
Q_OBJECT

public:
    TaskEditorWidget(TogMegFileModel * model, QWidget * parent = 0L);
    virtual ~TaskEditorWidget();

    TagMegFileEdit * view() const;
    
public slots:
    void addNewEntry();
    void setNextItemMode(TagMegFileEdit::NextItemMode);

private:
	class Private;
	Private * d;

};

#endif // TASKEDITORWIDGET_H

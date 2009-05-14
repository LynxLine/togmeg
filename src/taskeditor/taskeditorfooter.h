//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORFOOTER_H
#define TASKEDITORFOOTER_H

#include <QWidget>
#include "taskeditorview.h"

/*!
 * \class TaskEditorFooter
 */
class TaskEditorFooter : public QWidget
{
Q_OBJECT
public:
    TaskEditorFooter(QWidget * parent = 0);
    virtual ~TaskEditorFooter();

public slots:
    void setStudyCount(int);
    void setNextItemMode(TaskEditorView::NextItemMode);

signals:
    void nextItemModeChanged(TaskEditorView::NextItemMode);

private slots:
    void updateMessageLabel();
    void toQAQAMode();
    void toQQAAMode();

private:
    class Private;
    Private * d;
};

#endif // TASKEDITORFOOTER_H

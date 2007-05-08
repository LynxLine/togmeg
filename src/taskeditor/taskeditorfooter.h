//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TASKEDITORFOOTER_H
#define TASKEDITORFOOTER_H

#include <QWidget>

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
    void setCurrentTask(QString taskId);

private slots:
    void updateMessageLabel();

private:
    class Private;
    Private * d;
};

#endif // TASKEDITORFOOTER_H


#ifndef EXAMINEWIDGET_H
#define EXAMINEWIDGET_H

#include <QWidget>
#include "examinator.h"

class ExamineWidget : public QWidget
{
Q_OBJECT

public:
    ExamineWidget(Examinator *, QWidget * parent);
    virtual ~ExamineWidget();

protected:
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void setTaskName(QString name);
    void setExaminatorMode(Examinator::Mode);
    void setExaminatorState(Examinator::State);

private:
	class Private;
	Private * d;

};

#endif // EXAMINEWIDGET_H

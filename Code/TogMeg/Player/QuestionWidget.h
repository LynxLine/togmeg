
#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include "GradientWidget.h"

/*!
 * \class QuestionWidget
 */
class QuestionWidget : public GradientWidget
{
Q_OBJECT
public:
    QuestionWidget(QWidget * parent = 0);
    virtual ~QuestionWidget();

    QString question();

public slots:
    void setQuestion(QString);

private:
    class Private;
    Private * d;
};

#endif // QUESTIONWIDGET_H

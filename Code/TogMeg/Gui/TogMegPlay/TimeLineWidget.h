
#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

/*!
 * \class TimeLineWidget
 */
class TimeLineWidget : public QWidget
{
Q_OBJECT
public:
    TimeLineWidget(QWidget * parent = 0);
    virtual ~TimeLineWidget();

private:
    class Private;
    Private * d;
};

#endif // TIMELINEWIDGET_H

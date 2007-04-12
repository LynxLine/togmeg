//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef SLIDEDWIDGET_H
#define SLIDEDWIDGET_H

#include <QWidget>

/*!
 * \class SlidedWidget
 */
class SlidedWidget : public QWidget
{
Q_OBJECT
public:
    SlidedWidget(QWidget * parent = 0);
    virtual ~SlidedWidget();

    void addWidget(QWidget * widget);
    int count() const;
    int currentIndex() const;
    QWidget * currentWidget() const;
    int indexOf(QWidget * widget) const;
    QWidget * widget(int index) const;

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget * widget);

signals:
    void currentChanged(int index);

protected:
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void animateSwitching(int frame);
    void animateFinished();

private:
    class Private;
    Private * d;
};

#endif // SLIDEDWIDGET_H

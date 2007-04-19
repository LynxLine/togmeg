//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef F4BUTTON_H
#define F4BUTTON_H

#include <QAbstractButton>

/*!
 * \class PixmapButton
 */
class PixmapButton : public QAbstractButton
{
Q_OBJECT
public:
    PixmapButton(QWidget * parent =0);
    PixmapButton(QString pm, 
                 QString pmHl,
                 QString pmDown,
                 QWidget * parent =0);
    ~PixmapButton();

    QPixmap pixmap();
    QPixmap pixmapHl();
    QPixmap pixmapDown();

    void setPixmap(QPixmap);
    void setPixmapHl(QPixmap);
    void setPixmapDown(QPixmap);

    bool isHighlighted();

signals:
    void highlighted(bool);

public slots:
    void setHighlighted(bool);

protected:
    void enterEvent(QEvent * e);
    void leaveEvent(QEvent * e);
    void paintEvent(QPaintEvent * pe);
    
private:
    class Private;
    Private * d;
};

#endif


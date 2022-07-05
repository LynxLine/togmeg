
#ifndef PIXMAPBUTTON_H
#define PIXMAPBUTTON_H

#include <QAbstractButton>

/*!
 \class PixmapButton
 */
class PixmapButton : public QAbstractButton
{
Q_OBJECT
public:
    PixmapButton(QWidget * parent =0);
    PixmapButton(QPixmap pm, 
                 QPixmap pmHl,
                 QPixmap pmDown,
                 QPixmap pmOff = QPixmap(),
                 QWidget * parent =0);
    ~PixmapButton();

    QPixmap pixmap();
    QPixmap pixmapHl();
    QPixmap pixmapOff();
    QPixmap pixmapDown();

    bool isHighlighted();
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

signals:
    void highlighted(bool);
    void showMenu();

public slots:
    void setPixmap(QPixmap);
    void setPixmapHl(QPixmap);
    void setPixmapOff(QPixmap);
    void setPixmapDown(QPixmap);
    void setHighlighted(bool);

protected:
    void enterEvent(QEvent * e);
    void leaveEvent(QEvent * e);
    void paintEvent(QPaintEvent * pe);
    void mousePressEvent(QMouseEvent * me);
    void mouseReleaseEvent(QMouseEvent * me);
    
private:
    class Private;
    Private * d;
};

#endif // PIXMAPBUTTON_H

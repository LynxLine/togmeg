
#ifndef ProgressBar_H
#define ProgressBar_H

#include <QWidget>

class ProgressBar : public QWidget {
Q_OBJECT
public:
    ProgressBar(QWidget * parent = 0);
    virtual ~ProgressBar();

    QString title() const;
    void setTitle(const QString &title);

    int value() const;
    void setValue(int value);

    int minimum() const;
    int maximum() const;
    void setRange(int, int);

    void reset();
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *);

private:
    class Private;
    Private * d;
};

#endif // ProgressBar_H

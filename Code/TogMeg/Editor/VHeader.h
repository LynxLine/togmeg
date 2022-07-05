
#ifndef VHeader_H
#define VHeader_H

#include <QHeaderView>

class QVHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    QVHeaderView(Qt::Orientation, QWidget * parent =0L);
    virtual ~QVHeaderView();

protected:
    virtual void paintSection(QPainter *, const QRect &, int idx) const;
    virtual QSize sectionSizeFromContents(int idx) const;
};


#endif // VHeader_H

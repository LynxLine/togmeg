//
// Copyright OY 2010
//

#ifndef DecoratedItemDelegated_H
#define DecoratedItemDelegated_H

#include <QItemDelegate>

class DecoratedItemDelegate : public QItemDelegate
{
public:
    DecoratedItemDelegate(QAbstractItemView * parent);
    virtual ~DecoratedItemDelegate();

    // keys for use in variant map
    static const QString TextColor;
    static const QString OnMouseOver;
    static const QString MousePosRole;
    static const QString ProgressBrush;
    static const QString Progress;
    static const QString Margins;    
    static const QString Action;
    static const QString Align;
    static const QString Shift;
    static const QString Icon;
    static const QString Line;
    
    virtual QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
    virtual void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const;
    
protected:
    bool eventFilter(QObject *, QEvent *);
    void drawDisplay(QPainter *, const QStyleOptionViewItem &, const QRect &, const QString &) const;
    void drawCheck(QPainter *, const QStyleOptionViewItem &, const QRect &, Qt::CheckState) const;
    QPersistentModelIndex indexAboutToDraw() const;
    
private:
    class Private;
    Private * d;
};

#endif // DecoratedItemDelegated_H

//
// Copyright OY 2010
//

#include <QtGui>
#include "DecoratedItemDelegate.h"

const QString DecoratedItemDelegate::TextColor     = "TextColor";
const QString DecoratedItemDelegate::OnMouseOver   = "OnMouseOver"; 
const QString DecoratedItemDelegate::MousePosRole  = "MousePosRole";
const QString DecoratedItemDelegate::ProgressBrush = "ProgressBrush";
const QString DecoratedItemDelegate::Progress      = "Progress"; 
const QString DecoratedItemDelegate::Margins = "Margins"; 
const QString DecoratedItemDelegate::Action  = "Action";
const QString DecoratedItemDelegate::Align   = "Align";
const QString DecoratedItemDelegate::Shift   = "Shift";
const QString DecoratedItemDelegate::Icon    = "Icon";
const QString DecoratedItemDelegate::Line    = "Line";

class DecoratedItemDelegate::Private {
public:
    QAbstractItemView * view;
    QPersistentModelIndex indexAboutToDraw;
    QPersistentModelIndex hoverIndex;
    QPoint mousePos;
    
    QRect buttonRect(QVariant, QRect, int lines) const;
};

DecoratedItemDelegate::DecoratedItemDelegate(QAbstractItemView * view)
:QItemDelegate(view)
{
    d = new Private;
    d->view = view;
    view->installEventFilter(this);
    view->viewport()->installEventFilter(this);
}

DecoratedItemDelegate::~DecoratedItemDelegate()
{
    delete d;
}

bool DecoratedItemDelegate::eventFilter(QObject * obj, QEvent * e)
{
    if (e->type() == QEvent::Leave || 
        e->type() == QEvent::MouseMove || 
        e->type() == QEvent::MouseButtonPress || 
        e->type() == QEvent::MouseButtonRelease) {
        QMouseEvent * me = static_cast<QMouseEvent *>(e);
        
        if (d->hoverIndex.isValid())
            d->view->update(d->hoverIndex);
        
        d->hoverIndex = d->view->indexAt(me->pos());
        d->mousePos = me->pos();
        
        if (e->type() == QEvent::MouseButtonPress ||
            e->type() == QEvent::MouseButtonRelease) 
        {
            int lc = d->hoverIndex.data().toString().split("\n").count();
            if (lc <1) lc = 1;
            
            QVariant vd = d->hoverIndex.data(Qt::DecorationRole);
            if (!vd.isNull()) {
                
                QVariantList vl;
                if (vd.type() == QVariant::List)
                    vl = vd.toList();
                else vl.append(vd);
                
                foreach(QVariant v, vl) {
                    if (d->buttonRect(v, d->view->visualRect(d->hoverIndex), lc).contains(d->mousePos)) {
                        if (e->type() == QEvent::MouseButtonRelease) {            
                            if (!v.isNull()) {
                                QVariantMap vm = v.toMap();
                                int line = vm[Line].toInt();
                                int role = vm[Action].toInt();
                                if (int mousePosRole = vm[MousePosRole].toInt())
                                    d->view->model()->setData(d->hoverIndex, me->pos(), mousePosRole);
                                d->view->model()->setData(d->hoverIndex, line, role);
                            }
                        }
                        
                        d->view->update(d->hoverIndex);
                        return true;
                    }
                }
            }
        }
        
        d->view->update(d->hoverIndex);
    }
    
    return QItemDelegate::eventFilter(obj, e);
}

QSize DecoratedItemDelegate::sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    QSize s = QItemDelegate::sizeHint(o,i);
    
    QVariant vd = i.data(Qt::DecorationRole);
    if (!vd.isNull()) {
    
        QVariantList vl;
        if (vd.type() == QVariant::List)
            vl = vd.toList();
        else vl.append(vd);
    
        int l =0, t =0, r =0, b =0;

        foreach(QVariant v, vl) {
            if (v.toMap().contains(Margins)) {
                QVariantList vl = v.toMap()[Margins].toList();
                if (vl.count() == 4) {
                    l = qMax(l, vl[0].toInt());
                    t = qMax(t, vl[1].toInt());
                    r = qMax(r, vl[2].toInt());
                    b = qMax(b, vl[3].toInt());
                }
            }
        }
        s += QSize(l+r, t+b);
    }
    return s;
}

QRect DecoratedItemDelegate::Private::buttonRect(QVariant v, QRect orect, int lineCount) const
{
    QRect r;
    if (v.isNull()) return r;
    
    QVariantMap vm = v.toMap();
    if (vm[Icon].isNull()) return r;
    
    QSize s;

    if (vm[Icon].type() == QVariant::Icon) {
        QIcon icon = vm[Icon].value<QIcon>();
        if (!icon.availableSizes().count()) return r;
        s = icon.availableSizes().first();
    }
    else if (vm[Icon].type() == QVariant::Pixmap) {
        QPixmap pm = vm[Icon].value<QPixmap>();
        s = pm.size();
    }

    int h = orect.height() / lineCount;
    int l = vm[Line].toInt();
    
    int dy = (orect.height() * l) / lineCount;
    
    orect.setY(orect.y() + dy);
    orect.setHeight(h);
    
    r = QRect(QPoint(0,0), s);        
    r.moveCenter(orect.center());

    QVariantList vl = vm[Margins].toList();
    if (vl.count() == 4) {
        int lm = vl[0].toInt();
        int tm = vl[1].toInt();
        int rm = vl[2].toInt();
        int bm = vl[3].toInt();
        orect.adjust(lm,tm, -rm,-bm);

        int align = vm[Align].toInt();
        if (align == Qt::AlignRight)
            r.moveLeft(orect.right());
        else r.moveRight(orect.left()); 

        QPoint shift = vm[Shift].value<QPoint>();
        r.moveCenter(r.center()+shift);

        return r;
    }
    
    int align = vm[Align].toInt();
    if (align == Qt::AlignRight)
        r.moveRight(orect.right());
    else r.moveLeft(orect.left()); 
    
    QPoint shift = vm[Shift].value<QPoint>();
    r.moveCenter(r.center()+shift);

    return r;
}

void DecoratedItemDelegate::paint(QPainter * p, const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    d->indexAboutToDraw = i;
    
    QItemDelegate::paint(p, o, i);
    
    int lc = i.data().toString().split("\n").count();
    if (lc <1) lc = 1;

    QVariant vd = i.data(Qt::DecorationRole);
    if (vd.isNull()) return;

    QVariantList vl;
    if (vd.type() == QVariant::List)
        vl = vd.toList();
    else vl.append(vd);

    foreach(QVariant v, vl) {
        QVariantMap vm = v.toMap();
        
        if (vm[OnMouseOver].toBool() && i != d->hoverIndex)
            return;
        
        if (!vm[Progress].isNull()) {
            QRect r = o.rect;
            r.adjust(1,1, -1,-1);
            int progress = vm[Progress].toInt();
            r.setWidth(progress * r.width() / 100);
            
            if (!vm[ProgressBrush].isNull()) {
                QBrush b = vm[ProgressBrush].value<QBrush>();
                p->fillRect(r, b);
            }
            else p->fillRect(r, QColor(0,255,0, 128));
        }
        
        if (!vm[Icon].isNull()) {
            if (vm[Icon].type() == QVariant::Icon) {
                QIcon icon = vm[Icon].value<QIcon>();
                QRect r = d->buttonRect(v, d->view->visualRect(i), lc);

                QPixmap pm = icon.pixmap(r.size(), QIcon::Normal, QIcon::Off);
                if (!r.contains(d->mousePos)) {
                    if (o.state & QStyle::State_Selected && o.state & QStyle::State_Active)
                        pm = icon.pixmap(r.size(), QIcon::Selected, QIcon::Off);
                }
                else {
                    pm = icon.pixmap(r.size(), QIcon::Normal, QIcon::On);
                    if (o.state & QStyle::State_Selected && o.state & QStyle::State_Active)
                        pm = icon.pixmap(r.size(), QIcon::Selected, QIcon::On);
                }

                p->drawPixmap(r, pm);
            }
            else if (vm[Icon].type() == QVariant::Pixmap) {
                QPixmap pm = vm[Icon].value<QPixmap>();
                QRect r = d->buttonRect(v, d->view->visualRect(i), lc);
                p->drawPixmap(r, pm);
            }
        }
    }
}

void DecoratedItemDelegate::drawDisplay(QPainter * p, 
                                        const QStyleOptionViewItem & o, 
                                        const QRect & r, 
                                        const QString & t) const
{
    QRect rect = r;
    
    QVariant vd = d->indexAboutToDraw.data(Qt::DecorationRole);
    if (!vd.isNull()) {
    
        QVariantList vl;
        if (vd.type() == QVariant::List)
            vl = vd.toList();
        else vl.append(vd);
    
        int l =0, t =0, r =0, b =0;
    
        foreach(QVariant v, vl) {
            if (v.toMap().contains(Margins)) {
                QVariantList vl = v.toMap()[Margins].toList();
                if (vl.count() == 4) {
                    l = qMax(l, vl[0].toInt());
                    t = qMax(t, vl[1].toInt());
                    if (!v.toMap()[OnMouseOver].toBool() || d->indexAboutToDraw == d->hoverIndex)
                        r = qMax(r, vl[2].toInt());
                    b = qMax(b, vl[3].toInt());
                }
            }
        }
        rect.adjust(l,t,-r,-b);
    }
    
    return QItemDelegate::drawDisplay(p, o, rect, t);
}

void DecoratedItemDelegate::drawCheck(QPainter * p, 
                                      const QStyleOptionViewItem & o, 
                                      const QRect & r, 
                                      Qt::CheckState state) const
{
    QStyleOptionViewItemV4 oo = o;
    oo.state |= QStyle::State_Mini;
    return QItemDelegate::drawCheck(p, oo, r, state);
}

QPersistentModelIndex DecoratedItemDelegate::indexAboutToDraw() const
{
    return d->indexAboutToDraw;
}

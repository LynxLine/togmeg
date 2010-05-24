
#include <QtGui>
#include "FileNavigationView.h"
#include "FileNavigationModel.h"

#include "AppStyles.h"

class FileNavigationView::Private {
public:
    QPointer<FileNavigationModel> model;
    QPointer<FileNavigationViewDelegate> delegate;
    QModelIndex prevUnderMouse;
};

FileNavigationView::FileNavigationView(QWidget * parent)
:QTreeView(parent)
{
    d = new Private;
    
    setHeaderHidden(true);
    setMouseTracking(true);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAllColumnsShowFocus(true);
    setAlternatingRowColors(false);
    setContentsMargins(0,5,0,0);
    setEditTriggers(QAbstractItemView::SelectedClicked | 
                    QAbstractItemView::AnyKeyPressed);
    setAttribute(Qt::WA_MacShowFocusRect, false);
#ifdef Q_WS_MAC
    setFrameStyle(QFrame::NoFrame);
    setContentsMargins(1,60,1,1);
    setLineWidth(0);
#endif

    header()->setStretchLastSection(false);
    
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(214,221,229));
    setAutoFillBackground(true);
    setPalette(p);

    d->delegate = new FileNavigationViewDelegate(this);
    setItemDelegate(d->delegate);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(activateItem(const QModelIndex &)));    
}

FileNavigationView::~FileNavigationView()
{
    delete d;
}

void FileNavigationView::setModel(FileNavigationModel * m)
{
    d->model = m;
    QTreeView::setModel(m);
    header()->setResizeMode(FileNavigationModel::ColName, QHeaderView::Stretch);
    header()->setResizeMode(FileNavigationModel::ColLinks, QHeaderView::ResizeToContents);
}

void FileNavigationView::showEvent(QShowEvent * se)
{
    if (!se->spontaneous()) {
        if (!d->model) {
            setModel(new FileNavigationModel(this));
        }
    }
    QTreeView::showEvent(se);
}

void FileNavigationView::keyReleaseEvent(QKeyEvent * ke)
{
    if (ke->key() != Qt::Key_Return)
        return QTreeView::keyReleaseEvent(ke);
    
    QModelIndex i = currentIndex();

    if (!i.isValid()) return QTreeView::keyReleaseEvent(ke);
    if (!model()) return QTreeView::keyReleaseEvent(ke);
    
    FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
    if (!m) return QTreeView::keyReleaseEvent(ke);
    
    activateItem(i);
    ke->accept();
}

void FileNavigationView::activateItem(const QModelIndex & i)
{
    if (!i.isValid()) return;
    if (!model()) return;
    
    FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
    if (!m) return;
    
    QFileInfo * fi = m->fileInfo(i);
    if (fi->isDir()) {
        QString fileName = i.data().toString();
        return m->loadPathContent( m->path()+fileName );
    }
    
    emit openFileRequest(fi->absoluteFilePath());
}

void FileNavigationView::leaveEvent(QEvent * e)
{
    QTreeView::leaveEvent(e);
 
    if (!d->model) return;
   
    d->delegate->mousePos = QPoint(-1,-1);
    if (d->prevUnderMouse.isValid())
        update(d->prevUnderMouse);
}

void FileNavigationView::mouseMoveEvent(QMouseEvent * me)
{
    if (!d->model) return;

    d->delegate->mousePos = me->pos();
    QTreeView::mouseMoveEvent(me);
 
    if (d->prevUnderMouse.isValid())
        update(d->prevUnderMouse);
    
    QModelIndex mi = indexAt(me->pos());
    if (!mi.isValid()) return;
    update(mi);
    d->prevUnderMouse = mi;
}

void FileNavigationView::mousePressEvent(QMouseEvent * me)
{
    QTreeView::mousePressEvent(me);

    if (!d->model) return;

    QModelIndex mi = indexAt(me->pos());
    QString text = mi.data().toString();
    if (mi.column() == FileNavigationModel::ColLinks && !text.isEmpty()) {
        FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
        if (!m) return;

        QFileInfo * fi = m->fileInfo(mi);
        QString filePath = fi->filePath();
        filePath = filePath.mid(m->rootPath().length());
        if (fi->isDir()) filePath += "/";
        
        emit filterLinksByFilePath(filePath);
    }
}

void FileNavigationView::resizeEvent(QResizeEvent * re)
{
    QTreeView::resizeEvent(re);
    emit widthChanged(re->size().width());
}

//
// FileNavigationViewDelegate
//

class FileNavigationViewDelegate::Private {
public:
    QModelIndex i;
};

FileNavigationViewDelegate::FileNavigationViewDelegate(QObject * parent)
:QItemDelegate(parent)
{
    d = new Private;
}

FileNavigationViewDelegate::~FileNavigationViewDelegate()
{
    delete d;
}

QSize FileNavigationViewDelegate::sizeHint(const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    QSize s = QItemDelegate::sizeHint(o,i);
    if (s.height() < 20) s.setHeight(20);
    return s;
}
    
void FileNavigationViewDelegate::paint(QPainter * p, const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    d->i = i;
    QItemDelegate::paint(p, o, i);
    
    QString text = i.data().toString();
    if (d->i.column() == FileNavigationModel::ColLinks && !text.isEmpty()) {
        QFont f = AppStyles::systemFont();
        f.setBold(true);
        p->setFont(f);

        QFontMetrics fm(f);
        
        QRect r = fm.boundingRect(text);
        r.adjust(-5,0, 5,1);
        int w = r.width();
        int h = r.height();
        if (h>w) r.setWidth(h);
        r.moveCenter(o.rect.center());
        
        QColor c1 = QColor("#8797bb");
        if (o.rect.contains(mousePos))
            c1 = QColor(0,0,255);
        
        if (o.state & QStyle::State_Selected && o.state & QStyle::State_Active) {
            drawBox(p, r, Qt::white);
            p->setPen(c1);
            p->drawText(r, Qt::AlignCenter, text);
        }
        else {
            drawBox(p, r, c1);
            p->setPen(Qt::white);
            p->drawText(r, Qt::AlignCenter, text);
        }
    }
}

void FileNavigationViewDelegate::drawBox(QPainter * p, QRect r, QColor c) const
{
    p->setBrush(c);
    p->setPen(Qt::NoPen);
    p->setRenderHint(QPainter::Antialiasing);
    
    if (r.width() == r.height())
        return p->drawEllipse(r);
    
    QRect ro1(r.x(), r.y(), r.height(), r.height());
    p->drawEllipse(ro1);
    
    QRect ro2(r.x(), r.y(), r.height(), r.height());
    ro2.moveRight(r.right());
    p->drawEllipse(ro2);
    
    QRect rr = r;
    rr.setWidth(r.width()-r.height());
    rr.moveCenter(r.center());
    p->fillRect(rr, c);
}

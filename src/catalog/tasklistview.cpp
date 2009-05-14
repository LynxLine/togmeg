//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "crammero.h"
#include "studytask.h"
#include "mainwindow.h"
#include "headerview.h"
#include "tasklistview.h"
#include "tasklistmodel.h"

class TaskListView::Private
{
public:
    QPointer<TaskListModel> model;
    QPointer<QMenu> contextMenu;
    QPointer<HeaderView> header;

    QColor c1;
    QColor c2;
};

TaskListView::TaskListView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    d->header = new HeaderView(Qt::Horizontal, this);
    d->header->setFont( MainWindow::baseFont(0.95) );
    d->header->setStretchLastSection(true);

    setHeader( d->header );
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAutoFillBackground(true);
    verticalScrollBar()->setFixedWidth(15);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setAlternatingRowColors(true);

    setFrameStyle(QFrame::NoFrame);
    setItemDelegate(new TaskListItemDelegate(this));
    setEditTriggers(QAbstractItemView::EditKeyPressed);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Create new Study"), this, SLOT(addNewStudy()));
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("Rename"), this, SLOT(editStudyName()), QKeySequence("F2"));
    //d->contextMenu->addSeparator();
    //d->contextMenu->addAction( tr("Properties"));

    d->model = new TaskListModel(this);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), 
            this, SLOT(activateItem(const QModelIndex &)));
    connect(this, SIGNAL(activated(const QModelIndex &)), 
            this, SLOT(activateItem(const QModelIndex &)));

    setModel( d->model );
    if ( model()->rowCount() )
        setCurrentIndex( model()->index(0,0) );
}

TaskListView::~TaskListView()
{
    qDebug() << "~TaskListView";
    delete d;
}

void TaskListView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void TaskListView::addNewStudy()
{
    StudyTask * task = new StudyTask( d->model );
    d->model->addTask( task );

    QModelIndex index = d->model->indexOf(task);
    setCurrentIndex( index );
    edit( index );

    emit rowCountChanged( model()->rowCount() );
}

void TaskListView::editStudyName()
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;
    edit(index);
}

void TaskListView::editCurrentStudy()
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;
    activateItem(index);
}

void TaskListView::activateItem(const QModelIndex & i)
{
    if ( !i.isValid() ) return;
    QModelIndex index = i;
    if ( !index.isValid() ) return;

    StudyTask * task = d->model->task( index );
    emit studyTaskActivated( task->id() );
}

QString TaskListView::currentTaskId()
{
    QModelIndex current = currentIndex();
    if ( !current.isValid() ) {
        return QString::null;
    }

    QModelIndex index = current;

    if ( !index.isValid() ) {
        return QString::null;
    }

    StudyTask * task = d->model->task( index );
    return task->id();
}

void TaskListView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);

    if ( !current.isValid() ) {
        currentTaskChanged(QString::null);
        return;
    }

    QModelIndex index = current;

    if ( !index.isValid() ) {
        currentTaskChanged(QString::null);
        return;
    }

    StudyTask * task = d->model->task( index );
    emit currentTaskChanged( task->id() );
}

void TaskListView::paintEvent(QPaintEvent * pe)
{
    QTreeView::paintEvent(pe);
}

void TaskListView::drawRow(QPainter *painter, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QStyleOptionViewItemV2 opt = option;
    const int y = option.rect.y();
    const QModelIndex parent = index.parent();
    const QModelIndex current = currentIndex();
    const QStyle::State state = opt.state;

    const int left = 0;
    const int right = model()->columnCount()-1;

    const bool alternate = true;
    const bool enabled = (state & QStyle::State_Enabled) != 0;

    int width, height = option.rect.height();
    int position;
    int headerSection;
    QModelIndex modelIndex;

    QBrush fill;
    for (int headerIndex = left; headerIndex <= right; ++headerIndex) {
        headerSection = header()->logicalIndex(headerIndex);
        if (header()->isSectionHidden(headerSection))
            continue;

        position = columnViewportPosition(headerSection);
        width = header()->sectionSize(headerSection);
        modelIndex = model()->index(index.row(), headerSection, parent);
        opt.state = state;

        if (!modelIndex.isValid()) {
            opt.rect.setRect(position, y, width, height);
            painter->fillRect(opt.rect, palette().brush(QPalette::Base));
            continue;
        }

        if (enabled) {
            QPalette::ColorGroup cg;
            if ((d->model->flags(index) & Qt::ItemIsEnabled) == 0) {
                opt.state &= ~QStyle::State_Enabled;
                cg = QPalette::Disabled;
            } else {
                cg = QPalette::Active;
            }
            opt.palette.setCurrentColorGroup(cg);
        }

        if (alternate) {
            if (index.row() & 1) {
                opt.features |= QStyleOptionViewItemV2::Alternate;
                fill = opt.palette.brush(QPalette::AlternateBase);
            } else {
                opt.features &= ~QStyleOptionViewItemV2::Alternate;
                fill = opt.palette.brush(QPalette::Base);
            }
        }

        opt.rect.setRect(position, y, width, height);
        painter->fillRect(opt.rect, fill);

        {
            QRect r = opt.rect;
            if (headerIndex) {
                painter->drawLine(r.x(), r.y(), 
                                  r.x(), r.y()+r.height()
                );
            }
        }


        if (selectionModel()->isSelected(modelIndex)) {
            opt.state |= QStyle::State_Selected;
            if ( opt.state & QStyle::State_Active ) {
                QLinearGradient linearGradient(QPointF(0, y), QPointF(0, y+height));
                linearGradient.setColorAt(0, "#5F9CFE");
                linearGradient.setColorAt(1, "#1E61CD");

                painter->fillRect(opt.rect, linearGradient);
                QRect r = opt.rect;
                r.setHeight(1);
                painter->fillRect(r, QColor("#3B7CDA"));
                opt.palette.setBrush(QPalette::Active, QPalette::Highlight, linearGradient);
            }
            else {
                QLinearGradient linearGradient(QPointF(0, y), QPointF(0, y+height));
                linearGradient.setColorAt(0, "#A8B7CE");
                linearGradient.setColorAt(1, "#939CAB");

                painter->fillRect(opt.rect, linearGradient);
                QRect r = opt.rect;
                r.setHeight(1);
                painter->fillRect(r, QColor("#939CAB"));
                opt.palette.setBrush(QPalette::Inactive, QPalette::Highlight, linearGradient);
            }
            
            opt.palette.setColor(QPalette::Inactive, QPalette::Highlight, "#A8B7CE");
            opt.palette.setColor(QPalette::HighlightedText, "#FFFFFF");
        }

        opt.rect.adjust(5,0,0,0);
        itemDelegate()->paint(painter, opt, modelIndex);
    }
}

//
// Item Delegate
//

QSize TaskListItemDelegate::sizeHint(const QStyleOptionViewItem & o, const QModelIndex & index) const
{
    QSize s;
    s.setHeight(20);
    QFontMetrics fm(o.font);
    s.setWidth(fm.width( index.data(Qt::DisplayRole).toString())+10 );
    return s; 
}

void TaskListItemDelegate::paint(QPainter * p, const QStyleOptionViewItem & o, const QModelIndex & index) const
{
    p->save();

    //icon
    QPixmap pm;
    QRect drect;

    QVariant value = index.data(Qt::DecorationRole);
    if (value.isValid()) {
        if (value.type() == QVariant::Pixmap) {
            pm = qvariant_cast<QPixmap>(value);
            drect = QRect(QPoint(o.rect.x(), o.rect.y()), pm.size());
        }
    }
    p->drawPixmap(drect, pm);

    //text color
    QPalette::ColorGroup cg = o.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(o.state & QStyle::State_Active)) cg = QPalette::Inactive;
    
    if (o.state & QStyle::State_Selected) 
        p->setPen(o.palette.color(cg, QPalette::HighlightedText));
    else  p->setPen(o.palette.color(cg, QPalette::Text));

    int margin =4;
    QRect r = o.rect;
    r.adjust(drect.width()+margin,0, -margin,0);

    p->setFont( o.font );
    QString text = index.data(Qt::DisplayRole).toString();
    text = o.fontMetrics.elidedText(text, Qt::ElideRight, r.width());
    p->drawText( r, o.displayAlignment | index.data(Qt::TextAlignmentRole).toInt(), text);

    p->restore();
}

QWidget * TaskListItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    Q_UNUSED(o);
    QLineEdit * le = new QLineEdit(parent);
    le->setStyle( &app::cleanStyle );
    le->setFrame(false);
    {
        QPalette palette = le->palette();
        
        QBrush fill;
        if (i.row() & 1)  fill = parent->palette().brush(QPalette::AlternateBase);
        else fill = parent->palette().brush(QPalette::Base);
        
        palette.setBrush(QPalette::Base, fill);
        le->setPalette(palette);
    }
    parent->setFocusProxy(le);
    registerEditor(le);
    return le;
}

void TaskListItemDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    QPixmap pm;
    QRect drect;

    QVariant value = i.data(Qt::DecorationRole);
    if (value.isValid()) {
        if (value.type() == QVariant::Pixmap) {
            pm = qvariant_cast<QPixmap>(value);
            drect = QRect(QPoint(o.rect.x(), o.rect.y()), pm.size());
        }
    }

    QRect r = o.rect;
    r.adjust(drect.width()+2,2, -2,-2);
    editor->setGeometry( r );
}

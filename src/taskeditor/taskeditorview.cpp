//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "mainwindow.h"
#include "headerview.h"
#include "taskeditorview.h"
#include "studytaskmodel.h"

class TaskEditorView::Private
{
public:
    QPointer<StudyTaskModel> model;
    QPointer<QMenu> contextMenu;
    QPointer<HeaderView> header;

    QColor c1;
    QColor c2;
};

TaskEditorView::TaskEditorView(QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    d->header = new HeaderView(Qt::Horizontal, this);
    d->header->setFont( MainWindow::baseFont(0.95) );
    d->header->setStretchLastSection(true);

    setHeader( d->header );
    setRootIsDecorated(false);
    setAutoFillBackground(true);

#ifdef Q_WS_MAC
    setAlternatingRowColors(false);
    {
        d->c1 = palette().color(QPalette::Base);
        d->c2 = palette().color(QPalette::AlternateBase);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, QColor(0,0,0,0));
        setPalette(palette);
    }
#else 
    setAlternatingRowColors(true);
#endif

    setFrameStyle(QFrame::NoFrame);
    setAllColumnsShowFocus(true);
    setItemDelegate(new TaskEditorItemDelegate(this));
    setEditTriggers(
            QAbstractItemView::DoubleClicked |
            QAbstractItemView::EditKeyPressed |
            QAbstractItemView::AnyKeyPressed
        );

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Add new Question") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("&Remove"), this, SLOT(removeEntry()));

    d->model = new StudyTaskModel(this);
    setModel( d->model );

    header()->setDefaultAlignment(Qt::AlignLeft);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::Stretch);
    header()->setResizeMode(2, QHeaderView::Stretch);
}

TaskEditorView::~TaskEditorView()
{
    qDebug() << "~TaskEditorView";
    delete d;
}

void TaskEditorView::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void TaskEditorView::openTask(QString taskId)
{
    d->model->load(taskId);
    setFocus();
    if ( d->model->rowCount() )
        setCurrentIndex( d->model->index(0,0) );
}

void TaskEditorView::addNewEntry()
{
    QModelIndex index = d->model->addNewEntry();
    setCurrentIndex( index );
    edit( index );
}

void TaskEditorView::removeEntry()
{
    QModelIndex index = currentIndex();
    d->model->removeEntry( index );
    scrollTo(currentIndex());
}

void TaskEditorView::paintEvent(QPaintEvent * pe)
{
#ifdef Q_WS_MAC
    QRect r = pe->rect();
    QPainter p( viewport() );

    int rowHeight = 20;
    int i1 = r.y()/rowHeight-1;
    int i2 = (r.y()+r.height())/rowHeight+1;

    for (int i=i1;i<i2;i++) {
        QRect rf = QRect(r.x(), i*rowHeight, r.width(), rowHeight);
        p.fillRect(rf, i % 2 ? d->c2 : d->c1);
    }

    p.end();
#endif

    QTreeView::paintEvent(pe);
}

void TaskEditorView::drawRow(QPainter *painter, const QStyleOptionViewItem &option,
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
    const bool allColumnsShowFocus = true;

    bool currentRowHasFocus = false;
    if (allColumnsShowFocus && current.isValid()) { // check if the focus index is before or after the visible columns
        const int r = index.row();
        for (int c = 0; c < left && !currentRowHasFocus; ++c)
            currentRowHasFocus = (index.sibling(r, c) == current);
        for (int c = right; c < header()->count() && !currentRowHasFocus; ++c)
            currentRowHasFocus = (index.sibling(r, c) == current);
    }

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
        modelIndex = d->model->index(index.row(), headerSection, parent);
        opt.state = state;
        if (!modelIndex.isValid()) {
            opt.rect.setRect(position, y, width, height);
            painter->fillRect(opt.rect, palette().brush(QPalette::Base));
            continue;
        }

        if ((current == modelIndex) && hasFocus()) {
            if (allColumnsShowFocus)
                currentRowHasFocus = true;
            else
                opt.state |= QStyle::State_HasFocus;
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
            painter->setPen(QPen(QColor("#CCCCCC"), 1, Qt::SolidLine));
            painter->drawLine(r.x(),           r.y()+r.height()-1, 
                              r.x()+r.width(), r.y()+r.height()-1
            );

            if (headerIndex) {
                painter->drawLine(r.x(), r.y(), 
                                  r.x(), r.y()+r.height()
                );
            }
        }


        if (selectionModel()->isSelected(modelIndex)) {
            painter->fillRect(opt.rect, QColor("#BFC8D5"));

            opt.palette.setColor(QPalette::Inactive, QPalette::Highlight, "#A8B7CE");
            opt.palette.setColor(QPalette::HighlightedText, "#FFFFFF");

            /*
            if (current == modelIndex) {
                QRect r = opt.rect;
                r.setRect(r.x()+1,
                          r.y()+1,
                          r.width()-2,
                          r.height()-2
                );
                painter->fillRect(r, QColor("#FFFFFF"));
            }
            */

        }


        itemDelegate()->paint(painter, opt, modelIndex);
    }
}

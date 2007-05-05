//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "crammero.h"
#include "mainwindow.h"
#include "headerview.h"
#include "taskeditorview.h"
#include "studytaskmodel.h"

class TaskEditorView::Private
{
public:
    QPointer<StudyTaskModel> model;
    QPointer<TaskEditorItemDelegate> delegate;
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
    setAlternatingRowColors(true);
    setFrameStyle(QFrame::NoFrame);
    setAllColumnsShowFocus(true);
    verticalScrollBar()->setFixedWidth(15);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setStyle( &app::cleanStyle );
    setEditTriggers(
            QAbstractItemView::EditKeyPressed
            /*
            QAbstractItemView::DoubleClicked |
            QAbstractItemView::EditKeyPressed |
            QAbstractItemView::AnyKeyPressed
            */
        );

    d->delegate = new TaskEditorItemDelegate(this);
    connect(d->delegate, SIGNAL(returnPressed()), this, SLOT(editNextItem()));
    setItemDelegate( d->delegate );

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
    QTimer::singleShot(350, this, SLOT(toFirstRow()));
}

void TaskEditorView::toFirstRow()
{
    if ( d->model->rowCount() ) {
        QModelIndex index = model()->index(0,0);
        QMetaObject::invokeMethod(this, "setCurrentIndex", Qt::QueuedConnection, Q_ARG(QModelIndex, index));
    }
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
    QTreeView::paintEvent(pe);
}

void TaskEditorView::keyPressEvent(QKeyEvent * ke)
{
    if (ke->key() == Qt::Key_Up) {
        int previousRow = currentIndex().row();
        if (previousRow <= 0) return;
        previousRow--;

        QModelIndex previous = model()->index( previousRow, currentIndex().column() );
        setCurrentIndex(previous);
    }
    else if (ke->key() == Qt::Key_Down) {
        int nextRow = currentIndex().row();
        if (nextRow >= model()->rowCount()-1) return;
        nextRow++;

        QModelIndex next = model()->index( nextRow, currentIndex().column() );
        setCurrentIndex(next);
    }
    else
        QTreeView::keyPressEvent(ke);
}

void TaskEditorView::editNextItem()
{
    if ( !currentIndex().isValid() ) return;
    int nextRow = currentIndex().row();
    if ( currentIndex().column() == StudyTaskModel::QuestionColumn) {
        QModelIndex next = model()->index( nextRow, StudyTaskModel::AnswerColumn );
        setCurrentIndex(next);
    }
    else if ( currentIndex().column() == StudyTaskModel::AnswerColumn) {
        if (nextRow < model()->rowCount()-1)
        nextRow++;

        QModelIndex next = model()->index( nextRow, StudyTaskModel::QuestionColumn );
        setCurrentIndex(next);
    }
}

void TaskEditorView::editPreviousItem()
{
    if ( !currentIndex().isValid() ) return;
    int previousRow = currentIndex().row();
    if ( currentIndex().column() == StudyTaskModel::QuestionColumn) {
        if (previousRow > 0)
            previousRow--;

        QModelIndex previous = model()->index( previousRow, StudyTaskModel::AnswerColumn );
        setCurrentIndex(previous);
    }
    else if ( currentIndex().column() == StudyTaskModel::AnswerColumn) {
        QModelIndex previous = model()->index( previousRow, StudyTaskModel::QuestionColumn );
        setCurrentIndex(previous);
    }
}

void TaskEditorView::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
    //qDebug() << "TaskEditorView::closeEditor()" << hint;

    if (hint == QAbstractItemDelegate::EditNextItem) {
        QTreeView::closeEditor(editor, QAbstractItemDelegate::NoHint);
        editNextItem();
    }
    else if (hint == QAbstractItemDelegate::EditPreviousItem) {
        QTreeView::closeEditor(editor, QAbstractItemDelegate::NoHint);
        editPreviousItem();
    }
    else if (hint == QAbstractItemDelegate::RevertModelCache) {
        QTreeView::closeEditor(editor, QAbstractItemDelegate::RevertModelCache);
        QMetaObject::invokeMethod(this, "edit", Qt::QueuedConnection, Q_ARG(QModelIndex, currentIndex()));
    }
    else
        QTreeView::closeEditor(editor, hint);
}

void TaskEditorView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);
    if ( !current.isValid() ) return;

    if ( current.column() == StudyTaskModel::QuestionColumn ||
         current.column() == StudyTaskModel::AnswerColumn)
        edit(current);
    else {
        QModelIndex index = model()->index( current.row(), StudyTaskModel::QuestionColumn );
        setCurrentIndex(index);
    }
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

            if (current == modelIndex) {
                QRect r = opt.rect;
                r.setRect(r.x()+1,
                          r.y(),
                          r.width()-1,
                          r.height()-1
                );
                QColor c = index.row() & 1 ? 
                            opt.palette.color(QPalette::AlternateBase) : 
                            opt.palette.color(QPalette::Base);
                painter->fillRect(r, c);

                painter->save();
                painter->setPen(QPen(QColor("#A8B7CE"), 1, Qt::SolidLine));
                {
                    int x = r.x();
                    int y = r.y();
                    int w = r.width();
                    int h = r.height()-1;
                    
                    painter->drawLine(x,y, x+w,y);
                    painter->drawLine(x,y+h, x+w,y+h);
                }
                painter->restore();
            }

        }


        itemDelegate()->paint(painter, opt, modelIndex);
    }
}

//
// Item Delegate
//

QSize TaskEditorItemDelegate::sizeHint(const QStyleOptionViewItem & o, const QModelIndex & index) const
{
    QSize s;
    s.setHeight(20);
    QFontMetrics fm(o.font);
    s.setWidth(fm.width( index.data(Qt::DisplayRole).toString()+10 ));
    return s; 
}

void TaskEditorItemDelegate::paint(QPainter * p, const QStyleOptionViewItem & o, const QModelIndex & index) const
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
    p->drawText( r, o.displayAlignment, text);

    p->restore();
}

QWidget * TaskEditorItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const
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
    return le;
}

void TaskEditorItemDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const
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

void TaskEditorItemDelegate::setEditorData(QWidget * editor, const QModelIndex & i) const
{
    QVariant v = i.data(Qt::EditRole);
    QLineEdit * le = qobject_cast<QLineEdit *>(editor);
    if (!le) return;

    le->setText(v.toString());
}

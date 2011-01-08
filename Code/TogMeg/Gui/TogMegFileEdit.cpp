
#include <QtGui>
#include "TogMeg.h"
#include "TogMegWindow.h"
#include "TogMegFileEdit.h"
#include "TogMegFileModel.h"

#include "AppStyles.h"
#include "ColumnSelectorButton.h"

class TogMegFileEdit::Private
{
public:
    QPointer<TogMegFileModel> model;
    QPointer<TaskEditorItemDelegate> delegate;
    QPointer<QMenu> contextMenu;

    QColor c1;
    QColor c2;

    NextItemMode nextItemMode;
};

TogMegFileEdit::TogMegFileEdit(TogMegFileModel * model, QWidget * parent)
:QTreeView(parent) 
{
    d = new Private;
    d->model = model;
    d->nextItemMode = NextItemMode(-1); //undefined;

    setMouseTracking(true);
    setSortingEnabled(false);
    header()->setClickable(true);
    setRootIsDecorated(false);
    setAutoFillBackground(true);
    setAlternatingRowColors(true);
    setAllColumnsShowFocus(true);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
#ifdef Q_WS_MAC
    header()->setFixedHeight(QFontMetrics(AppStyles::systemFont()).height()+3);
    header()->setStyleSheet(AppStyles::mac_horHeaderStyle);
    setFrameStyle(QFrame::NoFrame);
    setContentsMargins(0,0,0,0);
    setLineWidth(0);

    setStyleSheet("QTreeView { \
                  border: 0px; \
                  }");
#endif
    
    //selector button in table
    QSize size;
    QStyleOptionHeader opt;
    size = style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, size, this);        
#ifdef Q_WS_MAC
    size.setHeight(QFontMetrics(AppStyles::systemFont()).height()+3);
#endif
    
    ColumnSelectorButton * b_columns = new ColumnSelectorButton(header());
    b_columns->setFixedHeight(size.height());
    b_columns->setPixmap(QPixmap());
    addScrollBarWidget(b_columns, Qt::AlignTop);
    
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setNextItemMode(QAQAMode);

    d->delegate = new TaskEditorItemDelegate(this);
    connect(d->delegate, SIGNAL(returnPressed()), this, SLOT(editNextItemUsingMode()));
    connect(d->delegate, SIGNAL(gotoAboveItem()), this, SLOT(editAboveItem()));
    connect(d->delegate, SIGNAL(gotoBelowItem()), this, SLOT(editBelowItem()));
    setItemDelegate( d->delegate );

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    d->contextMenu->addAction( tr("Add new Question") );
    d->contextMenu->addSeparator();
    d->contextMenu->addAction( tr("&Remove"), this, SLOT(removeEntry()));

    setModel( model );

    header()->setDefaultAlignment(Qt::AlignLeft);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::Stretch);
    header()->setResizeMode(2, QHeaderView::Stretch);
    
    connect(model, SIGNAL(modelAboutToBeReset()), this, SLOT(modelAboutToBeReset()));
    toFirstRow();
}

TogMegFileModel * TogMegFileEdit::studyTaskModel() const
{
    return d->model;
}

TogMegFileEdit::~TogMegFileEdit()
{
    delete d;
}

TogMegFileEdit::NextItemMode TogMegFileEdit::nextItemMode()
{
    return d->nextItemMode;
}

void TogMegFileEdit::setNextItemMode(NextItemMode m)
{
    if ( d->nextItemMode == m ) return;
    d->nextItemMode = m;

    emit nextItemModeChanged(m);
}

void TogMegFileEdit::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void TogMegFileEdit::toFirstRow()
{
    if ( d->model->rowCount() ) {
        QModelIndex index = model()->index(1, TogMegFileModel::ColQ);
        QMetaObject::invokeMethod(this, "setCurrentIndex", Qt::QueuedConnection, Q_ARG(QModelIndex, index));
    }
}

void TogMegFileEdit::addNewEntry()
{
    QModelIndex index = d->model->addNewEntry();
    setCurrentIndex( index );
}

void TogMegFileEdit::removeEntry()
{
    QModelIndex index = currentIndex();
    d->model->removeEntry( index );
    scrollTo(currentIndex());
}

void TogMegFileEdit::swapQA()
{
    QModelIndex index = currentIndex();
    d->model->swapQA( index );
    scrollTo(currentIndex());
}

void TogMegFileEdit::keyPressEvent(QKeyEvent * ke)
{
    if (ke->key() == Qt::Key_Up) {
        int previousRow = currentIndex().row();
        if (previousRow <= 0) return;
        previousRow--;

        QModelIndex previous = model()->index( previousRow, currentIndex().column() );
        setCurrentIndex(previous);
        return ke->accept();
    }
    else if (ke->key() == Qt::Key_Down) {
        int nextRow = currentIndex().row();
        if (nextRow >= model()->rowCount()-1) return;
        nextRow++;

        QModelIndex next = model()->index( nextRow, currentIndex().column() );
        setCurrentIndex(next);
        return ke->accept();
    }
    
    return QTreeView::keyPressEvent(ke);
}

void TogMegFileEdit::editNextItemUsingMode()
{
    //qDebug() << "editNextItemUsingMode()" << nextItemMode();

    if ( nextItemMode() == QAQAMode ) {
        editNextItem();
    }
    else if ( nextItemMode() == QQAAMode ) {
        if ( !currentIndex().isValid() ) return;

        int nextRow = currentIndex().row();
        if (nextRow >= model()->rowCount()-1) {
            edit( currentIndex() ); //just reopen
            return;
        }

        nextRow++;
        QModelIndex next = model()->index( nextRow, currentIndex().column() );
        setCurrentIndex(next);
    }
}

void TogMegFileEdit::editNextItem()
{
    if ( !currentIndex().isValid() ) return;
    int nextRow = currentIndex().row();
    if ( currentIndex().column() == TogMegFileModel::ColQ) {
        QModelIndex next = model()->index( nextRow, TogMegFileModel::ColA );
        setCurrentIndex(next);
    }
    else if ( currentIndex().column() == TogMegFileModel::ColA) {
        if (nextRow < model()->rowCount()-1)
            nextRow++;

        QModelIndex next = model()->index( nextRow, TogMegFileModel::ColQ );
        setCurrentIndex(next);
    }
}

void TogMegFileEdit::editAboveItem()
{
    if ( !currentIndex().isValid() ) return;
    int row = currentIndex().row() -1;
    int col = currentIndex().column();
    if (row <1) return;

    QModelIndex next = model()->index( row, col );
    setCurrentIndex(next);
}

void TogMegFileEdit::editBelowItem()
{
    if ( !currentIndex().isValid() ) return;
    int row = currentIndex().row() +1;
    int col = currentIndex().column();
    if (row > model()->rowCount()-1) return;

    QModelIndex next = model()->index( row, col );
    setCurrentIndex(next);
}

void TogMegFileEdit::editPreviousItem()
{
    if ( !currentIndex().isValid() ) return;
    int previousRow = currentIndex().row();
    if ( currentIndex().column() == TogMegFileModel::ColQ) {
        if (previousRow > 1)
            previousRow--;

        QModelIndex previous = model()->index( previousRow, TogMegFileModel::ColA );
        setCurrentIndex(previous);
    }
    else if ( currentIndex().column() == TogMegFileModel::ColA) {
        QModelIndex previous = model()->index( previousRow, TogMegFileModel::ColQ );
        setCurrentIndex(previous);
    }
}

void TogMegFileEdit::closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint)
{
    //qDebug() << "TogMegFileEdit::closeEditor()" << hint;

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

void TogMegFileEdit::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //qDebug() << "TogMegFileEdit::currentChanged" << current.row() << current.column();

    QTreeView::currentChanged(current, previous);
    if ( !current.isValid() ) return;

    if ( current.column() == TogMegFileModel::ColQ ||
         current.column() == TogMegFileModel::ColA)
        edit(current);
    else {
        QModelIndex index = model()->index( current.row(), TogMegFileModel::ColQ );
        setCurrentIndex(index);
    }
}

void TogMegFileEdit::drawRow(QPainter *painter, const QStyleOptionViewItem &option,
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
            if (index.row() == 0) {
                fill = QColor(225,255,225);
            }
            else if (index.row() & 1) {
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
                painter->drawLine(r.x()-1, r.y(), 
                                  r.x()-1, r.y()+r.height()
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

                    if ( headerSection == TogMegFileModel::ColA ) {
                        painter->drawLine(x+w-1,y, x+w-1,y+h);
                    }
                }
                painter->restore();
            }

        }


        itemDelegate()->paint(painter, opt, modelIndex);
    }
}

void TogMegFileEdit::modelAboutToBeReset()
{
    setCurrentIndex(QModelIndex()); 
}

//
// Item Delegate
//

QSize TaskEditorItemDelegate::sizeHint(const QStyleOptionViewItem & o, const QModelIndex & index) const
{
    QSize s;
    s.setHeight(20);
    QFontMetrics fm(o.font);
    s.setWidth(fm.width( index.data(Qt::DisplayRole).toString())+10 );
    return s; 
}

void TaskEditorItemDelegate::paint(QPainter * p, const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    p->save();

    //icon
    QPixmap pm;
    QRect drect;

    QVariant v = i.data(Qt::DecorationRole);
    if (v.isValid()) {
        if (v.type() == QVariant::Pixmap) {
            pm = v.value<QPixmap>();
            drect = QRect(QPoint(o.rect.x(), o.rect.y()), pm.size());
        }
        else if (v.type() == QVariant::Icon) {
            QIcon ic = v.value<QIcon>();
            int h = o.rect.height()-6;
            pm = ic.pixmap(h,h);
            drect = QRect(QPoint(o.rect.width()-h,
                                 o.rect.y()+3),
                          pm.size());
        }
    }
    p->drawPixmap(drect, pm);

    //text color
    QPalette::ColorGroup cg = o.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(o.state & QStyle::State_Active)) cg = QPalette::Inactive;
    
    if (o.state & QStyle::State_Selected) 
        p->setPen(o.palette.color(cg, QPalette::HighlightedText));
    else  p->setPen(o.palette.color(cg, QPalette::Text));

    int margin =6;
    QRect r = o.rect;
    if (drect.width())
        r.adjust(drect.width()+margin,0, -margin,0);

    v = i.data(Qt::FontRole);
    if (v.isValid())
        p->setFont(v.value<QFont>());
    else p->setFont( o.font );

    QString text = i.data(Qt::DisplayRole).toString();
    text = o.fontMetrics.elidedText(text, Qt::ElideRight, r.width());
    p->drawText( r, o.displayAlignment | i.data(Qt::TextAlignmentRole).toInt(), text);

    p->restore();
}

QWidget * TaskEditorItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const
{
    Q_UNUSED(o);
    if (i.row() > 0) {
        TaskItemEditor * le = new TaskItemEditor(parent);
        //le->setStyle( &app::cleanStyle );
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

        le->setNoAbove(i.row() <= 1);
        le->setNoBelow(i.row() >= i.model()->rowCount()-1);

        connect(le, SIGNAL(editNextItem()), this, SLOT(editNextItem()));
        connect(le, SIGNAL(editAboveItem()), this, SLOT(editAboveItem()));
        connect(le, SIGNAL(editBelowItem()), this, SLOT(editBelowItem()));
        connect(le, SIGNAL(editPreviousItem()), this, SLOT(editPreviousItem()));
        return le;
    }
    else if (i.row() == 0) {
        QComboBox * cb = new QComboBox(parent);
        cb->setFont(AppStyles::systemFont());
        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(editNextItem()));
        return cb;
    }

    return TogMegFileDelegate::createEditor(parent, o, i);
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

    int m = 1;
    QRect r = o.rect;
    if (i.row() >0) {
        r.adjust(drect.width()+m,m, -m,-m);
        if (i.column()) r.adjust(0,0,-1,0);
        editor->setGeometry(r);
    }
    else {
        r.adjust(0,0,0,-m);
        editor->setGeometry(r);
    }
}

void TaskEditorItemDelegate::setEditorData(QWidget * editor, const QModelIndex & i) const
{
    QVariant v = i.data(Qt::EditRole);
    if (i.row() > 0) {
        QLineEdit * le = qobject_cast<QLineEdit *>(editor);
        if (!le) return;

        le->setText(v.toString());
    }
    else if (i.row() == 0) {
        QComboBox * cb = qobject_cast<QComboBox *>(editor);
        if (!cb) return;

        cb->clear();
        cb->blockSignals(true);

        QVariantMap vm = v.toMap();
        QString current = i.data(Qt::DisplayRole).toString();
        QVariant currentData;

        cb->addItem(tr("None"), QVariant());
        cb->insertSeparator(cb->count());

        foreach(QString name, vm.keys()) {
            cb->addItem(name, vm[name]);
            if (name == current)
                currentData = vm[name];
        }

        if (!currentData.isNull())
            cb->setCurrentIndex(cb->findData(currentData));

        cb->blockSignals(false);
    }
}

void TaskEditorItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * m, const QModelIndex & i) const
{
    if (i.row() >0)
        return TogMegFileDelegate::setModelData(editor, m, i);

    if (i.row() == 0) {
        QComboBox * cb = qobject_cast<QComboBox *>(editor);
        if (!cb) return;

        QVariant v = cb->itemData(cb->currentIndex());
        m->setData(i, v);
    }
}


void TaskEditorItemDelegate::editNextItem()
{
    QWidget * editor = ::qobject_cast<QWidget*>(sender());
    if (!editor) return;

    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
}

void TaskEditorItemDelegate::editAboveItem()
{
    QWidget * editor = ::qobject_cast<QWidget*>(sender());
    if (!editor) return;

    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::NoHint);
    emit gotoAboveItem();
}

void TaskEditorItemDelegate::editBelowItem()
{
    QWidget * editor = ::qobject_cast<QWidget*>(sender());
    if (!editor) return;

    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::NoHint);
    emit gotoBelowItem();
}

void TaskEditorItemDelegate::editPreviousItem()
{
    QWidget * editor = ::qobject_cast<QWidget*>(sender());
    if (!editor) return;

    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::EditPreviousItem);
}

void TaskItemEditor::setNoAbove(bool f)
{
    noAbove = f;
}

void TaskItemEditor::setNoBelow(bool f)
{
    noBelow = f;
}

void TaskItemEditor::keyPressEvent(QKeyEvent * ke)
{
    QLineEdit::keyPressEvent(ke);

    if ( !selectedText().isEmpty() ||
            ke->modifiers() & Qt::AltModifier ||
            ke->modifiers() & Qt::ShiftModifier ||
            ke->modifiers() & Qt::ControlModifier)
        return;

    if ( cursorPosition() == 0 && ke->key() == Qt::Key_Left)
        emit editPreviousItem();

    if ( cursorPosition() == text().length() && ke->key() == Qt::Key_Right)
        emit editNextItem();

    if (ke->key() == Qt::Key_Up && !noAbove)
        emit editAboveItem();

    if (ke->key() == Qt::Key_Down && !noBelow)
        emit editBelowItem();
}

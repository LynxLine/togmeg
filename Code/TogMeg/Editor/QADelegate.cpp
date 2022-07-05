
#include <QtGui>
#include "QADelegate.h"

#include "AppStyles.h"

QWidget * TaskEditorItemDelegate::createEditor(QWidget * parent,
                                               const QStyleOptionViewItem & o,
                                               const QModelIndex & i) const
{
    Q_UNUSED(o);
    if (i.row() > 0) {
        QALineEdit * le = new QALineEdit(parent);
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

        connect(le, SIGNAL(editNextItem()), this, SLOT(editNext()));
        connect(le, SIGNAL(editPrevItem()), this, SLOT(editPrev()));
        connect(le, SIGNAL(editAboveItem(int)), this, SLOT(editAbove(int)));
        connect(le, SIGNAL(editBelowItem(int)), this, SLOT(editBelow(int)));
        return le;
    }
    else if (i.row() == 0) {
        QComboBox * cb = new QComboBox(parent);
        cb->setFont(AppStyles::systemFont());
        connect(cb, SIGNAL(activated(int)), this, SLOT(gotoNext()));
        return cb;
    }

    return QItemDelegate::createEditor(parent, o, i);
}

void TaskEditorItemDelegate::updateEditorGeometry(QWidget * editor,
                                                  const QStyleOptionViewItem & o,
                                                  const QModelIndex & i) const
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

void TaskEditorItemDelegate::setEditorData(QWidget * e, const QModelIndex & i) const
{
    QVariant v = i.data(Qt::EditRole);
    if (i.row() > 0) {
        if (QLineEdit * le = qobject_cast<QLineEdit *>(e))
            le->setText(v.toString());
    }
    else if (i.row() == 0) {
        QComboBox * cb = qobject_cast<QComboBox *>(e);
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
        cb->showPopup();
    }
}

void TaskEditorItemDelegate::setModelData(QWidget * editor,
                                          QAbstractItemModel * m,
                                          const QModelIndex & i) const
{
    if (i.row() >0) {
        QByteArray n = editor->metaObject()->userProperty().name();
        if (!n.isEmpty())
            m->setData(i, editor->property(n), Qt::EditRole);
    }

    if (i.row() == 0) {
        QComboBox * cb = qobject_cast<QComboBox *>(editor);
        if (!cb) return;

        QVariant v = cb->itemData(cb->currentIndex());
        m->setData(i, v);
    }
}

void TaskEditorItemDelegate::setPosInLineEdit(int pos) {
    if (editors.empty()) return;
    if (QLineEdit * le = qobject_cast<QLineEdit *>(editors.back())) {
        le->deselect();
        le->setCursorPosition(pos);
    }
}

void TaskEditorItemDelegate::gotoNext() {
    QWidget * e = ::qobject_cast<QWidget*>(sender());
    if (!e) return;

    emit commitData(e);
    emit closeEditor(e, QAbstractItemDelegate::NoHint);
    emit gotoNextItem();
}

void TaskEditorItemDelegate::editNext() {
    QWidget * e = ::qobject_cast<QWidget*>(sender());
    if (!e) return;

    emit commitData(e);
    emit closeEditor(e, QAbstractItemDelegate::EditNextItem);
}

void TaskEditorItemDelegate::editAbove(int pos) {
    QWidget * e = ::qobject_cast<QWidget*>(sender());
    if (!e) return;

    emit commitData(e);
    emit closeEditor(e, QAbstractItemDelegate::NoHint);
    emit editAboveItem(pos);
}

void TaskEditorItemDelegate::editBelow(int pos) {
    QWidget * e = ::qobject_cast<QWidget*>(sender());
    if (!e) return;

    emit commitData(e);
    emit closeEditor(e, QAbstractItemDelegate::NoHint);
    emit editBelowItem(pos);
}

void TaskEditorItemDelegate::editPrev() {
    QWidget * e = ::qobject_cast<QWidget*>(sender());
    if (!e) return;

    emit commitData(e);
    emit closeEditor(e, QAbstractItemDelegate::EditPreviousItem);
}

void TaskEditorItemDelegate::registerEditor(QWidget * e) const {
    connect(e, SIGNAL(destroyed(QObject *)),
            this, SLOT(deactivateEditor(QObject *)));
    editors << e;
}

void TaskEditorItemDelegate::deactivateEditor(QObject * o)
{
    QWidget * e = ::qobject_cast<QWidget*>(o);
    if (!editors.contains(e)) return;
    editors.removeAll(e);
}

bool TaskEditorItemDelegate::eventFilter(QObject * o, QEvent * ev)
{
    QWidget * e = ::qobject_cast<QWidget*>(o);
    if (!e) return QItemDelegate::eventFilter(o,ev);

    if (!editors.contains(e))
        return QItemDelegate::eventFilter(o,ev);

    if (ev->type() == QEvent::KeyPress) {
        switch (static_cast<QKeyEvent *>(ev)->key()) {
        case Qt::Key_Tab:
            emit commitData(e);
            emit closeEditor(e, QAbstractItemDelegate::EditNextItem);
            return true;
        case Qt::Key_Backtab:
            emit commitData(e);
            emit closeEditor(e, QAbstractItemDelegate::EditPreviousItem);
            return true;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            // We want the editor to be able to process the key press
            // before committing the data (e.g. so it can do
            // validation/fixup of the input).
            QMetaObject::invokeMethod(this,
                                      "commitDataAndCloseEditor",
                                      Qt::QueuedConnection, Q_ARG(QWidget *, e));
            return false;
        case Qt::Key_Escape:
            emit closeEditor(e, QAbstractItemDelegate::RevertModelCache);
            break;
        default:
            return false;
        }
        if (view) {
            view->setFocus();
        }
        return true;
    }
    else if (ev->type() == QEvent::FocusOut) {
        if (!e->isActiveWindow() || (QApplication::focusWidget() != e)) {
            QWidget * w = QApplication::focusWidget();
            while (w) {
                // don't worry about focus changes internally in the editor
                if (w == e) {
                    return false;
                }
                w = w->parentWidget();
            }

            // Opening a modal dialog will start a new eventloop
            // that will process the deleteLater e.
            if (QApplication::activeModalWidget() &&
                !QApplication::activeModalWidget()->isAncestorOf(e))
                return false;

            return false;
        }
    }
    return false;
}

void TaskEditorItemDelegate::commitDataAndCloseEditor(QWidget * e) {
    emit commitData(e);
    emit closeEditor(e, QAbstractItemDelegate::SubmitModelCache);
    emit returnPressed();
}

//
// TaskItemEditor
//

void QALineEdit::setNoAbove(bool f) { noAbove = f; }
void QALineEdit::setNoBelow(bool f) { noBelow = f; }

void QALineEdit::keyPressEvent(QKeyEvent * ke)
{
    if ( !selectedText().isEmpty() ||
            ke->modifiers() & Qt::AltModifier ||
            ke->modifiers() & Qt::ShiftModifier ||
            ke->modifiers() & Qt::ControlModifier)
        return QLineEdit::keyPressEvent(ke);

    int pos = cursorPosition();
    QLineEdit::keyPressEvent(ke);

    if      (ke->key() == Qt::Key_Up   && !noAbove) emit editAboveItem(pos);
    else if (ke->key() == Qt::Key_Down && !noBelow) emit editBelowItem(pos);
    else if (pos == 0               && ke->key() == Qt::Key_Left) emit editPrevItem();
    else if (pos == text().length() && ke->key() == Qt::Key_Right) emit editNextItem();
}

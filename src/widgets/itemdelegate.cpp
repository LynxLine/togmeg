//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include "crammero.h"
#include "itemdelegate.h"

void ItemDelegate::registerEditor(QWidget * editor) const
{
    connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(deactivateEditor(QObject *)));
    editors << editor;
}

void ItemDelegate::deactivateEditor(QObject * o)
{
    QWidget * editor = ::qobject_cast<QWidget*>(o);
    if ( !editors.contains(editor) ) return;
    editors.removeAll(editor);
}

void ItemDelegate::setEditorData(QWidget * editor, const QModelIndex & i) const
{
    QVariant v = i.data(Qt::EditRole);
    QByteArray n = editor->metaObject()->userProperty().name();
    if (!n.isEmpty())
        editor->setProperty(n, v);
}

void ItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & i) const
{
    QByteArray n = editor->metaObject()->userProperty().name();
    if (!n.isEmpty())
        model->setData(i, editor->property(n), Qt::EditRole);
}

bool ItemDelegate::eventFilter(QObject * o, QEvent * e)
{
    QWidget * editor = ::qobject_cast<QWidget*>(o);
    if (!editor)
        return false;

    if (!editors.contains(editor))
        return false;

    if (e->type() == QEvent::KeyPress) {
        switch (static_cast<QKeyEvent *>(e)->key()) {
        case Qt::Key_Tab:
            emit commitData(editor);
            emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
            return true;
        case Qt::Key_Backtab:
            emit commitData(editor);
            emit closeEditor(editor, QAbstractItemDelegate::EditPreviousItem);
            return true;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            // We want the editor to be able to process the key press
            // before committing the data (e.g. so it can do
            // validation/fixup of the input).
            QMetaObject::invokeMethod(this, 
                                      "commitDataAndCloseEditor",
                                      Qt::QueuedConnection, Q_ARG(QWidget *, editor));
            return false;
        case Qt::Key_Escape:
            emit closeEditor(editor, QAbstractItemDelegate::RevertModelCache);
            break;
        default:
            return false;
        }
        if (editor->parentWidget())
            editor->parentWidget()->setFocus();
        return true;
    } 
    else if (e->type() == QEvent::FocusOut) {
        if (!editor->isActiveWindow() || (QApplication::focusWidget() != editor)) {
            QWidget * w = QApplication::focusWidget();
            while (w) { 
                // don't worry about focus changes internally in the editor
                if (w == editor) {
                    return false;
                }
                w = w->parentWidget();
            }

            // Opening a modal dialog will start a new eventloop
            // that will process the deleteLater e.
            if (QApplication::activeModalWidget() && !QApplication::activeModalWidget()->isAncestorOf(editor))
                return false;

            //qDebug() << "full focus out";
            return false;

            //emit commitData(editor);
            //emit closeEditor(editor, NoHint);
        }
    }
    return false;
}

void ItemDelegate::commitDataAndCloseEditor(QWidget * editor)
{
    emit commitData(editor);
    emit closeEditor(editor, QAbstractItemDelegate::SubmitModelCache);
    emit returnPressed();
}


#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QAbstractItemView>
#include <QAbstractItemDelegate>
class QAbstractItemModel;

/*!
 * \class TogMegFileDelegate
 */
class TogMegFileDelegate : public QAbstractItemDelegate
{
Q_OBJECT
public:
    TogMegFileDelegate(QAbstractItemView * parent):QAbstractItemDelegate(parent), view(parent) {;}
    virtual bool eventFilter(QObject * object, QEvent * event);

    //editor
    virtual void registerEditor(QWidget * editor) const;

    //data
    virtual void setEditorData(QWidget * editor, const QModelIndex & i) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & i) const;

signals:
    void returnPressed();

private slots:
    void deactivateEditor(QObject * editor);
    void commitDataAndCloseEditor(QWidget * editor);

private:
    mutable QList<QWidget *> editors;
    QAbstractItemView * view;
};

#endif // ITEMDELEGATE_H
   
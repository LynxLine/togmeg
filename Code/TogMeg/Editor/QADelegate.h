
#ifndef QADelegate_H
#define QADelegate_H

#include <QLineEdit>
#include <QItemDelegate>
#include <QAbstractItemView>

class TaskEditorItemDelegate : public QItemDelegate {
    Q_OBJECT
    QAbstractItemView * view;
    mutable QList<QWidget *> editors;
public:
    TaskEditorItemDelegate(QAbstractItemView * p = 0):QItemDelegate(p), view(p) {;}
    virtual ~TaskEditorItemDelegate() {}

    virtual void setEditorData(QWidget * editor, const QModelIndex & i) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * m, const QModelIndex & i) const;
    virtual QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & o, const QModelIndex & i) const;

signals:
    void gotoNextItem();
    void editAboveItem(int pos);
    void editBelowItem(int pos);
    void returnPressed();

private slots:
    void gotoNext();
    void editNext();
    void editPrev();
    void editAbove(int pos);
    void editBelow(int pos);
    void setPosInLineEdit(int);

    virtual bool eventFilter(QObject * object, QEvent * event);
    virtual void registerEditor(QWidget * editor) const;

    void deactivateEditor(QObject *);
    void commitDataAndCloseEditor(QWidget *);
};

class QALineEdit : public QLineEdit {
    Q_OBJECT
    bool noAbove, noBelow;
public:
    QALineEdit(QWidget * parent = 0L)
        :QLineEdit(parent), noAbove(false), noBelow(false)
    { setAlignment(Qt::AlignCenter); }

    void setNoAbove(bool);
    void setNoBelow(bool);

signals:
    void editNextItem();
    void editPrevItem();
    void editAboveItem(int pos);
    void editBelowItem(int pos);

protected:
    virtual void keyPressEvent(QKeyEvent * ke);
};

#endif // QADelegate_H

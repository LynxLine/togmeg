
#ifndef QAEditor_H
#define QAEditor_H

#include <QTableView>
#include <QItemDelegate>

#include "TogMeg.h"

class QAModel;

class QAEditor : public QTableView {
    Q_OBJECT
	class Private;
	Private * d;
public:
    QAEditor(QAModel *, QWidget * parent = 0);
    virtual ~QAEditor();

    enum NextItemMode {
        QAQAMode = 0,
        QQAAMode
    };
    NextItemMode mode() const;
    QModelIndex next(QModelIndex) const;
    QModelIndex prev(QModelIndex) const;

signals:
    void setPosInLineEdit(int pos);

public slots:
    void cut();
    void copy();
    void paste();
    void pastePlainData(QString);
    void pasteDataInRow(QModelIndex currentInRow, QString rowData, int & right);

    void swapQA();
    void addNewEntry();
    void removeSelection();
    void setMode(QAEditor::NextItemMode);

protected slots:
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint);

private slots:
    void toFirstRow();
    void modelAboutToBeReset();
    void onDoubleClick(QModelIndex);
    void activateContextMenu(const QPoint &);

    void gotoNext();
    void gotoPrev();
    void gotoNextByMode();
    void gotoPrevByMode();

    void editNext();
    void editPrev();
    void editAbove(int pos);
    void editBelow(int pos);
    void editNextByMode();
};

#endif // QAEditor_H

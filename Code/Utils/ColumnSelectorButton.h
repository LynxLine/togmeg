//
// Copyright Petrostreamz 2009
//

#ifndef COLUMNSELECTORBUTTON_H
#define COLUMNSELECTORBUTTON_H

#include <QToolButton>
class QHeaderView;

/*!
 \brief Button to be shown in header of table for selecting columns.
 */
class ColumnSelectorButton : public QToolButton {
    Q_OBJECT
public:
    ColumnSelectorButton(QHeaderView * h, QWidget * parent = 0L);
    virtual ~ColumnSelectorButton();
    
public slots:
    void showContentMenu();
    void setContentMenu(QMenu *);
    void setPixmap(QPixmap);
    
signals:
    void doubleClicked();
    
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    
private:
    class Private;
    Private * d;    
};

#endif // COLUMNSELECTORBUTTON_H

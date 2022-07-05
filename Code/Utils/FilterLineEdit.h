//
// Copyright Petrostreamz 2008
//

#ifndef FILTERLINEEDIT_H
#define FILTERLINEEDIT_H

#include <QLineEdit>

class FilterLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    FilterLineEdit(QWidget * parent =0L);
    virtual ~FilterLineEdit();
    
    void setProgressVisible(bool);
    void setProgress(qint64 done, qint64 total);
    
signals:
    void optionsClicked();
    
protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent * pe);

private:
    class Private;
    Private * d;
};

#endif // FILTERLINEEDIT_H

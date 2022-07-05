//
// Copyright Petrostreamz 2008
//

#ifndef LABELWITHPROGRESS_H
#define LABELWITHPROGRESS_H

#include <QLabel>

class LabelWithProgress : public QLabel
{
    Q_OBJECT
public:
    LabelWithProgress(QWidget * parent =0L);
    LabelWithProgress(QString text, QWidget * parent =0L);
    virtual ~LabelWithProgress();

    void setProgressVisible(bool);
    void setProgress(qint64 done, qint64 total);
    
protected:
    void paintEvent(QPaintEvent * pe);
    
private:
    class Private;
    Private * d;
};

#endif // LABELWITHPROGRESS_H

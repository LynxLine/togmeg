//
// Copyright Petrostreamz 2008
//

#ifndef LINEEDITWITHPROGRESS_H
#define LINEEDITWITHPROGRESS_H

#include <QLineEdit>

class LineEditWithProgress : public QLineEdit
{
    Q_OBJECT
public:
    LineEditWithProgress(QWidget * parent =0L);
    virtual ~LineEditWithProgress();
    
    void setProgressVisible(bool);
    void setProgress(qint64 done, qint64 total);
    
protected:
    void paintEvent(QPaintEvent * pe);
    
private:
    class Private;
    Private * d;
};

#endif // LINEEDITWITHPROGRESS_H

//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "LineEditWithProgress.h"

class LineEditWithProgress::Private {
public:
    bool showProgress;
    qint64 done, total;
};

LineEditWithProgress::LineEditWithProgress(QWidget * parent)
:QLineEdit(parent)
{
    d = new Private;  
    d->showProgress = false;
    d->total = 100;
    d->done = 0;
}

LineEditWithProgress::~LineEditWithProgress()
{
    delete d;
}

void LineEditWithProgress::setProgressVisible(bool f)
{
    d->showProgress = f;
    update();
}

void LineEditWithProgress::setProgress(qint64 done, qint64 total)
{
    d->done = done;
    d->total = total;
    update();
}

void LineEditWithProgress::paintEvent(QPaintEvent * pe)
{
    QLineEdit::paintEvent(pe);
    if (d->showProgress && d->total >0) {
        QPainter p(this);
        int x = d->done * qint64(width()) / qint64(d->total);
        p.fillRect(QRect(0,0, x, height()), QColor(59,135,224, 80));
        p.end();
    }    
}

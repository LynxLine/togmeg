//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "LabelWithProgress.h"

class LabelWithProgress::Private {
public:
    bool showProgress;
    qint64 done, total;
};

LabelWithProgress::LabelWithProgress(QWidget * parent)
:QLabel(parent)
{
    d = new Private;  
    d->showProgress = false;
    d->total = 100;
    d->done = 0;
}

LabelWithProgress::LabelWithProgress(QString text, QWidget * parent)
:QLabel(text, parent)
{
    d = new Private;
}

LabelWithProgress::~LabelWithProgress()
{
    delete d;
}

void LabelWithProgress::setProgressVisible(bool f)
{
    d->showProgress = f;
    update();
}

void LabelWithProgress::setProgress(qint64 done, qint64 total)
{
    d->done = done;
    d->total = total;
    update();
}

void LabelWithProgress::paintEvent(QPaintEvent * pe)
{
    if (d->showProgress && d->total >0) {
        QPainter p(this);
        int x = d->done * qint64(width()) / qint64(d->total);
        p.fillRect(QRect(0,0, x, height()), QColor("#3b87e0"));
        p.end();
    }
    QLabel::paintEvent(pe);
}

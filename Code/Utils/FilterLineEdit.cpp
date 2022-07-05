//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "FilterLineEdit.h"

#include "IconSet.h"
#include "PixmapButton.h"

class FilterLineEdit::Private {
public:
    PixmapButton * tb_close;
    PixmapButton * tb_options;
    
    bool showProgress;
    qint64 done, total;
};

FilterLineEdit::FilterLineEdit(QWidget * parent)
:QLineEdit(parent)
{
    d = new Private;
    
    d->showProgress = false;
    d->total = 100;
    d->done = 0;
    
    d->tb_close = new PixmapButton(this);
    d->tb_close->setPixmap(EmbPixmap("CloseIcon"));
    d->tb_close->setPixmapHl(EmbPixmap("CloseIcon"));
    d->tb_close->setPixmapDown(EmbPixmap("CloseIcon"));

    d->tb_options = new PixmapButton(this);
    d->tb_options->setPixmap(EmbPixmap("SearchOptionsDropDown"));
    d->tb_options->setPixmapHl(EmbPixmap("SearchOptionsDropDown"));
    d->tb_options->setPixmapDown(EmbPixmap("SearchOptionsDropDown"));
    
    connect(d->tb_close, SIGNAL(clicked()), this, SLOT(clear()));
    connect(d->tb_options, SIGNAL(clicked()), this, SIGNAL(optionsClicked()));
    
    setMinimumHeight(d->tb_close->pixmap().height()+4);
    int fm = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    setStyleSheet(QString("QLineEdit { padding-left: %1px; padding-right: %2px; } ")
                  .arg(d->tb_options->sizeHint().width() + fm + 2)
                  .arg(d->tb_close->sizeHint().width() + fm + 1));
}

FilterLineEdit::~FilterLineEdit()
{
    delete d;
}

void FilterLineEdit::resizeEvent(QResizeEvent *)
{
    QSize sz1 = d->tb_options->sizeHint();    
    QSize sz2 = d->tb_close->sizeHint();    
    
    int fm = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    
    d->tb_options->move(rect().left() + fm+1,
                        (rect().bottom() + 1 - sz1.height())/2);
    d->tb_close->move(rect().right() - fm - sz2.width(),
                      (rect().bottom() + 1 - sz2.height())/2);
}

void FilterLineEdit::setProgressVisible(bool f)
{
    d->showProgress = f;
    update();
}

void FilterLineEdit::setProgress(qint64 done, qint64 total)
{
    d->done = done;
    d->total = total;
    update();
}

void FilterLineEdit::paintEvent(QPaintEvent * pe)
{
    QLineEdit::paintEvent(pe);
    if (d->showProgress && d->total >0) {
        QPainter p(this);
        int x = d->done * qint64(width()) / qint64(d->total);
        p.fillRect(QRect(0,0, x, height()), QColor(59,135,224, 80));
        p.end();
    }    
}

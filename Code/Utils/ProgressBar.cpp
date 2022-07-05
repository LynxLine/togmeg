
#include <QtGui>
#include "ProgressBar.h"
#include "AppStyles.h"

#define PROGRESSBAR_INDENT 6
#define PROGRESSBAR_HEIGHT 12

class ProgressBar::Private {
public:
    Private():  minimum(1), maximum(100), value(1) {;}
    QImage bar;
    QString text;
    QString title;
    int progressHeight;
    int minimum;
    int maximum;
    int value;

    void drawCornerImage(const QImage &img, QPainter *painter, QRect rect,
                         int left, int top, int right, int bottom);

};

ProgressBar::ProgressBar(QWidget * parent)
:QWidget(parent)
{
    d = new Private;
    setFont(AppStyles::systemFont());
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setMouseTracking(true);
}

ProgressBar::~ProgressBar()
{
    delete d;
}

void ProgressBar::reset()
{
    d->value = d->minimum;
    update();
}

void ProgressBar::setRange(int minimum, int maximum)
{
    d->minimum = minimum;
    d->maximum = maximum;
    if (d->value < d->minimum || d->value > d->maximum)
        d->value = d->minimum;
    update();
}

void ProgressBar::setValue(int value)
{
    if (d->value == value ||
        value < d->minimum ||
        value > d->maximum) {
        return;
    }
    d->value = value;
    update();
}

int ProgressBar::value() const { return d->value; }
int ProgressBar::minimum() const { return d->minimum; }
int ProgressBar::maximum() const { return d->maximum; }

QString ProgressBar::title() const
{
    return d->title;
}

void ProgressBar::setTitle(const QString &title)
{
    if (d->title == title)
        return;

    d->title = title;
    update();
}

QSize ProgressBar::sizeHint() const
{
    QSize s;
    s.setWidth(50);
    s.setHeight(PROGRESSBAR_HEIGHT);
    return s;
}

void ProgressBar::paintEvent(QPaintEvent *)
{
    if (d->bar.isNull())
        d->bar.load(QLatin1String(":/Images/ProgressBarLong.png"));

    double range = maximum() - minimum();
    double percent = 0.;
    if (range != 0)
        percent = (value() - minimum()) / range;
    if (percent > 1)
        percent = 1;
    else if (percent < 0)
        percent = 0;

    QPainter p(this);
    QFontMetrics fm = p.fontMetrics();

    int w = fm.width(d->title);
    int wmin = fm.width("W")*15;
    if (w < wmin) w = wmin;

    d->progressHeight = PROGRESSBAR_HEIGHT;
    d->progressHeight += ((d->progressHeight % 2) + 1) % 2; // make odd

    // draw outer rect
    QRect p_rect(PROGRESSBAR_INDENT - 1+w, 0, size().width()-2*PROGRESSBAR_INDENT + 1-w, d->progressHeight-1);
    p.setPen(Qt::white);
    d->drawCornerImage(d->bar, &p, p_rect, 2, 2, 2, 2);

    // draw inner rect
    QColor c = QColor(120,150,255);
    c.setAlpha(180);
    p.setPen(Qt::NoPen);

    QRect inner = p_rect.adjusted(3, 2, -2, -2);
    inner.adjust(0, 0, qRound((percent - 1) * inner.width()), 0);
    if (value() == 100 && range != 0) {
        c = QColor(90, 170, 60);
    }

    // Draw line and shadow after the gradient fill
    if (value() > 0 && value() < maximum()) {
        p.fillRect(QRect(inner.right() + 1, inner.top(), 2, inner.height()), QColor(0, 0, 0, 20));
        p.fillRect(QRect(inner.right() + 1, inner.top(), 1, inner.height()), QColor(0, 0, 0, 60));
    }
    QLinearGradient grad(inner.topLeft(), inner.bottomLeft());
    grad.setColorAt(0, c.lighter(130));
    grad.setColorAt(0.5, c.lighter(106));
    grad.setColorAt(0.51, c.darker(106));
    grad.setColorAt(1, c.darker(130));
    p.setPen(Qt::NoPen);
    p.setBrush(grad);
    p.drawRect(inner);
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(0, 0, 0, 30), 1));
    p.drawLine(inner.topLeft(), inner.topRight());
    p.drawLine(inner.topLeft(), inner.bottomLeft());
    p.drawLine(inner.topRight(), inner.bottomRight());
    p.drawLine(inner.bottomLeft(), inner.bottomRight());
    p.drawPoint(inner.bottomLeft());
    p.drawPoint(inner.bottomRight());

    int alignment = Qt::AlignVCenter | Qt::AlignRight;

    QRect textRect = rect().adjusted(1,0,0,0);
    textRect.setWidth(w+2);

    p.setPen(Qt::white);
    p.drawText(textRect, alignment, d->title);
    p.translate(0, -1);
    p.setPen(Qt::black);
    p.drawText(textRect, alignment, d->title);

}

void ProgressBar::Private::drawCornerImage(const QImage &img, QPainter *painter, QRect rect, int l, int t, int r, int b)
{
    QSize size = img.size();
    if (t > 0) { //top
        painter->drawImage(QRect(rect.left() + l, rect.top(), rect.width() -r - l, t), img,
                           QRect(l, 0, size.width() -r - l, t));
        if (l > 0) //top-left
            painter->drawImage(QRect(rect.left(), rect.top(), l, t), img,
                               QRect(0, 0, l, t));
        if (r > 0) //top-right
            painter->drawImage(QRect(rect.left() + rect.width() - r, rect.top(), r, t), img,
                               QRect(size.width() - r, 0, r, t));
    }
    //left
    if (l > 0)
        painter->drawImage(QRect(rect.left(), rect.top()+t, l, rect.height() - t - b), img,
                           QRect(0, t, l, size.height() - b - t));
    //center
    painter->drawImage(QRect(rect.left() + l, rect.top()+t, rect.width() -r - l,
                             rect.height() - b - t), img,
                       QRect(l, t, size.width() -r -l,
                             size.height() - b - t));
    if (r > 0) //right
        painter->drawImage(QRect(rect.left() +rect.width() - r, rect.top()+t, r, rect.height() - t - b), img,
                           QRect(size.width() - r, t, r, size.height() - b - t));
    if (b > 0) { //bottom
        painter->drawImage(QRect(rect.left() +l, rect.top() + rect.height() - b,
                                 rect.width() - r - l, b), img,
                           QRect(l, size.height() - b,
                                 size.width() - r - l, b));
    if (l > 0) //bottom-left
        painter->drawImage(QRect(rect.left(), rect.top() + rect.height() - b, l, b), img,
                           QRect(0, size.height() - b, l, b));
    if (r > 0) //bottom-right
        painter->drawImage(QRect(rect.left() + rect.width() - r, rect.top() + rect.height() - b, r, b), img,
                           QRect(size.width() - r, size.height() - b, r, b));
    }
}

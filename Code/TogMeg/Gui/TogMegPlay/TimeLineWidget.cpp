
#include <QtGui>
#include "TogMegWindow.h"
#include "TimelineWidget.h"

#include "AppStyles.h"

class TimeLineWidget::Private {
public:
    QLabel * l_reaction;
    QLabel * l_reading;
    QLabel * l_thinking;
    QLabel * l_typing;
    QLabel * l_validating;
    QLabel * l_total;
};

/*!
 * Creates the object.
 */
TimeLineWidget::TimeLineWidget(QWidget * parent)
:QWidget(parent)
{
    d = new Private;

    setFont(AppStyles::systemFont());

    QGridLayout * layout = new QGridLayout;
    layout->setSpacing(5);
    layout->setMargin(0);
    setLayout(layout);

    QLabel * l = new QLabel(tr("<b>Timeline</b>"));

    QHBoxLayout * hbox = new QHBoxLayout;
    hbox->setSpacing(2);
    hbox->setMargin(0);
    hbox->addWidget(l);

    {
        QFrame * hline = new QFrame;
        hline->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        hline->setFrameStyle(QFrame::HLine | QFrame::Sunken);
        hline->setMidLineWidth(0);
        hline->setLineWidth(1);
        hbox->addWidget( hline );
    }

    layout->addLayout( hbox, 0,0, 1,2);
    layout->addItem(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Fixed), 1,0);

    layout->addWidget( new QLabel(tr("Reaction")),      2,0);
    layout->addWidget( new QLabel(tr("Reading words")), 3,0);
    layout->addWidget( new QLabel(tr("Thinking")),      4,0);
    layout->addWidget( new QLabel(tr("Typing answer")), 5,0);
    layout->addWidget( new QLabel(tr("Validating")),    6,0);

    d->l_reaction   = new QLabel(tr("0.2"));
    d->l_reading    = new QLabel(tr("1 X 0.5"));
    d->l_thinking   = new QLabel(tr("2.0"));
    d->l_typing     = new QLabel(tr("10 X 1.0"));
    d->l_validating = new QLabel(tr("3.0"));
    d->l_total      = new QLabel(tr("<b>12.0</b>"));

    d->l_reaction   ->setAlignment(Qt::AlignRight);
    d->l_reading    ->setAlignment(Qt::AlignRight);
    d->l_thinking   ->setAlignment(Qt::AlignRight);
    d->l_typing     ->setAlignment(Qt::AlignRight);
    d->l_validating ->setAlignment(Qt::AlignRight);
    d->l_total      ->setAlignment(Qt::AlignRight);

    layout->addWidget( d->l_reaction,   2,1);
    layout->addWidget( d->l_reading,    3,1);
    layout->addWidget( d->l_thinking,   4,1);
    layout->addWidget( d->l_typing,     5,1);
    layout->addWidget( d->l_validating, 6,1);

    QFrame * hline = new QFrame;
    {
        QPalette palette = hline->palette();
        palette.setColor(QPalette::WindowText, "#A0A0A0");
        hline->setPalette(palette);
        hline->setFrameStyle(QFrame::HLine | QFrame::Plain);
        hline->setFixedHeight(1);
        hline->setLineWidth(1);
    }

    layout->addItem(new QSpacerItem(10,5, QSizePolicy::Minimum, QSizePolicy::Fixed), 7,0);
    layout->addWidget( hline, 8,0, 1,2 );
    layout->addItem(new QSpacerItem(10,5, QSizePolicy::Minimum, QSizePolicy::Fixed), 9,0);

    layout->addWidget( new QLabel(tr("<b>Total<b>")), 10,0);
    layout->addWidget( d->l_total,                    10,1);
}

/*!
 * Deletes it.
 */
TimeLineWidget::~TimeLineWidget()
{
    delete d;
}

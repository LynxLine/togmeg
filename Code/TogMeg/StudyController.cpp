
#include <QtGui>
#include <ctime>

#include "TogMeg.h"
#include "QAModel.h"
#include "StudyController.h"

class StudyController::Private {
public:
    Private():idx(1),isLastFailed(false) {;}

    int idx;
    std::vector<int> sequence;

    bool isLastFailed;
    QTimeLine * timeLine;
    ControllerDataEntry currentEntry;
};

/*!
 * Creates the object.
 */
StudyController::StudyController(QAModel * m)
:TaskController(m)
{
    d = new Private;

    std::vector<int> idxs;
    srand(unsigned(time(NULL)));

    //take all
    for (int i =1; i< model->rowCount(); i++) {
        QModelIndex mi = model->index(i, QAModel::ColQ);
        QString q = model->data(mi).toString();
        if (!q.isEmpty())
            idxs.push_back(i);
    }

    std::random_shuffle(idxs.begin(), idxs.end());

    //make filler for last serie
    std::vector<int> last = idxs;
    std::random_shuffle(last.begin(), last.end());
    if (last.size() >12) {
        std::vector<int> temp = last;
        last.assign(temp.begin(), temp.begin()+12);
    }

    //make series of 12
    for (int s =0; s< int(idxs.size())/12 +1; s++) {
        std::vector<int> sub;
        uint to = (s+1)*12;
        sub.assign(idxs.begin()+s*12, idxs.begin()+(to <= idxs.size() ? to : idxs.size()));
        if (sub.size() <12) {
            sub.insert(sub.end(), last.begin(), last.end());
            if (sub.size() >12) {
                std::vector<int> temp = sub;
                sub.assign(temp.begin(), temp.begin()+12);
            }
        }

        for (int i =0; i< 3; i++) {
            std::random_shuffle(sub.begin(), sub.end());
            d->sequence.insert(d->sequence.end(), sub.begin(), sub.end());
        }
    }

    d->timeLine = new QTimeLine(3000, this);
    d->timeLine->setCurveShape(QTimeLine::LinearCurve);
    d->timeLine->setFrameRange(0,100);
    d->timeLine->setLoopCount(1);

    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(readyForNext()), Qt::QueuedConnection);
}

/*!
 * Deletes it.
 */
StudyController::~StudyController()
{
    delete d;
}

/*! Check that at least one question is available
    in model. Iterate through all items O(n)
  */
bool StudyController::hasNext()
{
    return !d->sequence.empty();
}

ControllerDataEntry StudyController::next()
{
    QString q;
    if (!d->isLastFailed) {
        d->idx = d->sequence.back();
        d->sequence.pop_back();
    }

    ControllerDataEntry e;

    QModelIndex miq = model->index(d->idx, QAModel::ColQ);
    QModelIndex mia = model->index(d->idx, QAModel::ColA);

    e.a = model->data(mia).toString();
    e.q = model->data(miq).toString();

    int typingSpeed = app::typingSpeed(); //symbols in minute
    if ( typingSpeed <= 0 ) typingSpeed = 60;

    int typingTime = e.a.length() *1000 *60 /typingSpeed;

    e.startTime = 0;
    e.totalTime = 5000 + typingTime;

    model->setData(miq, true, QAModel::SpeechRole);

    d->currentEntry = e;
    return e;
}

void StudyController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(usedTime);
    
    if ( d->timeLine->state() != QTimeLine::NotRunning ) {
        d->timeLine->setCurrentTime( d->timeLine->duration() );
        d->timeLine->stop();
        readyForNext();
        return;
    }

    if ( d->currentEntry.a != answer.toLower().simplified() ) {
        d->isLastFailed = !d->isLastFailed;

        emit indicateMismatching();
        d->timeLine->setDuration( 2000 ); // timeline to end of speech
    }
    else {
        d->isLastFailed = false;

        emit indicateMatching();
        d->timeLine->setDuration( 2000 ); // timeline to end of speech

        //commit stats
        QList<int> times = eventTimeMap.keys();
        if (times.count() >= 2) {
            qSort(times);
            int msecs = times.last() - times.first();
            if (msecs) {
                app::addTypingStats(msecs, times.count());
            }
        }
    }

    QModelIndex mi = model->index(d->idx, QAModel::ColA);
    model->setData(mi, true, QAModel::SpeechRole);

    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void StudyController::readyForNext()
{
    emit requestNextQuestion();
}

//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#ifdef Q_WS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifdef Q_WS_WIN
#include <windows.h>
#include <atlbase.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sapi.h>
#include <sphelper.h>
#endif

#include "crammero.h"
#include "studytaskmodel.h"
#include "studytaskcontroller.h"

class StudyTaskController::Private {
public:
    Private():index(0) {;}

    int index;
    QTimeLine * timeLine;
    ControllerDataEntry currentEntry;
    
#ifdef Q_WS_MAC
    SpeechChannel speechChannel;
#endif

#ifdef Q_WS_WIN
    CComPtr<ISpVoice> cpVoice;
#endif
};

/*!
 * Creates the object.
 */
StudyTaskController::StudyTaskController(StudyTaskModel * parent)
:TaskController(parent)
{
    d = new Private;

    d->timeLine = new QTimeLine(1000, this);
    d->timeLine->setCurveShape(QTimeLine::LinearCurve);
    d->timeLine->setFrameRange(0,100);
    d->timeLine->setLoopCount(1);

    connect(d->timeLine, SIGNAL(finished()),
            this, SLOT(readyForNext()), Qt::QueuedConnection);
    
#ifdef Q_WS_MAC
    OSErr theErr = noErr;
    theErr = NewSpeechChannel(NULL, &d->speechChannel);
    if (theErr != noErr) {
        qDebug() << "NewSpeechChannel() failed," << theErr;
        return;
    }
#endif

#ifdef Q_WS_WIN
    HRESULT hr;
    hr = d->cpVoice.CoCreateInstance( CLSID_SpVoice );
    if (!SUCCEEDED(hr)) 
        return; 

    QMainWindow * mw = qobject_cast<QMainWindow *>(qApp->activeWindow());
    if (!mw) {
        qDebug() << "No active main window";
        return;
    }

    ULONGLONG events = SPFEI(SPEI_WORD_BOUNDARY) | SPFEI(SPEI_END_INPUT_STREAM);
    
    d->cpVoice->SetInterest( events, events );
    d->cpVoice->SetNotifyWindowMessage( mw->winId(), WM_USER+5767, 0, 0 );
    //d->initialized = true;   

	//if (voiceId.isValid())
	//	setVoice(voiceId);
#endif
}

/*!
 * Deletes it.
 */
StudyTaskController::~StudyTaskController()
{
#ifdef Q_WS_MAC
    if (!d->speechChannel) return;
    
    OSErr theErr = noErr;
    theErr = StopSpeech(d->speechChannel);
    if (theErr != noErr)
        qDebug() << "StopSpeech() failed," << theErr;
    
    theErr = DisposeSpeechChannel(d->speechChannel);
    if (theErr != noErr)
        qDebug() << "DisposeSpeechChannel() failed," << theErr;
#endif
    delete d;
}

bool StudyTaskController::hasNext()
{
    int nextIndex = 0;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, StudyTaskModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry StudyTaskController::next()
{
    QString question;
    while ( question.isEmpty() ) {
        d->index = rand() % model->rowCount();
        QModelIndex i = model->index(d->index, StudyTaskModel::QuestionColumn);
        question = model->data(i).toString();
    }

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, StudyTaskModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index, StudyTaskModel::QuestionColumn) ).toString();

    int typingSpeed = app::typingSpeed(); //symbols in minute
    if ( typingSpeed <= 0 ) typingSpeed = 60;

    int typingTime = entry.answer.length() *1000 *60 /typingSpeed;

    entry.startTime = 0;
    entry.totalTime = 5000 + typingTime;

    d->currentEntry = entry;
 
    return entry;
}

void StudyTaskController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(usedTime);
    
    if ( d->timeLine->state() != QTimeLine::NotRunning ) {
        d->timeLine->setCurrentTime( d->timeLine->duration() );
        d->timeLine->stop();
        readyForNext();
        return;
    }

    if ( d->currentEntry.answer != answer.toLower().simplified() ) {
        emit indicateMismatching();
        d->timeLine->setDuration( 1000 );
    }
    else {
        emit indicateMatching();
        d->timeLine->setDuration( 300 );

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
    
    QString text = d->currentEntry.answer;
#ifdef Q_WS_MAC
    SpeakText(d->speechChannel , text.toLatin1().data(), text.toLatin1().size() );
#endif

#ifdef Q_WS_WIN
	WCHAR * wcText = new WCHAR[ text.length()+1 ];
    text.toWCharArray(wcText);
    wcText[text.length()] = 0;
    //d->textLength = text.length();

    HRESULT hr = d->cpVoice->Speak( wcText, SPF_ASYNC | SPF_IS_NOT_XML, 0 );
    if (FAILED( hr )) qDebug() << "sapi51: Speak() is not completed";
    delete[] wcText;
#endif

    d->timeLine->setCurrentTime(0);
    d->timeLine->start();
}

void StudyTaskController::readyForNext()
{
    emit requestNextQuestion();
}

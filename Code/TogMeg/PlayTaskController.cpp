
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

#include "TogMegFileModel.h"
#include "PlayTaskController.h"

class PlayTaskController::Private {
public:
    Private():index(0) {;}

    int index;
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
PlayTaskController::PlayTaskController(TogMegFileModel * parent)
:TaskController(parent)
{
    d = new Private;
    
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
PlayTaskController::~PlayTaskController()
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

bool PlayTaskController::hasNext()
{
    int nextIndex = d->index;
    while ( nextIndex < model->rowCount() ) {
        QModelIndex i = model->index(nextIndex, TogMegFileModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        nextIndex++;
    }
    return nextIndex < model->rowCount();
}

ControllerDataEntry PlayTaskController::next()
{
    while ( d->index < model->rowCount() ) {
        QModelIndex i = model->index(d->index, TogMegFileModel::QuestionColumn);
        QString question = model->data(i).toString();
        if ( !question.isEmpty() ) break;
        d->index++;
    }

    ControllerDataEntry entry;

    entry.answer = model->data( model->index(d->index, TogMegFileModel::AnswerColumn) ).toString();
    entry.question = model->data( model->index(d->index, TogMegFileModel::QuestionColumn) ).toString();

    entry.totalTime = 5000; //temp
    entry.startTime = 0; //temp

    QString text = entry.answer;

#ifdef Q_WS_MAC
    QTextCodec * tc = QTextCodec::codecForName("Apple Roman");
    QByteArray enc = tc->fromUnicode(text);    
    SpeakText(d->speechChannel , enc.data(), enc.size() );
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

    d->index++;

    return entry;
}

void PlayTaskController::processAnswer(int usedTime, QString answer)
{
    Q_UNUSED(usedTime);
    Q_UNUSED(answer);
    emit requestNextQuestion();
}

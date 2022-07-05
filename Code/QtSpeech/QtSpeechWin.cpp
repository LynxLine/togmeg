//
// Author Oleksandr Iakovliev <yshurik@gmail.com>
//

#include <QtGui>
#include "sapi51channel.h"

#include <windows.h>
#include <atlbase.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sapi.h>
#include <sphelper.h>

namespace QtSpeech1 { // API v1.0

class QtSpeechWin::Private {
public:
    bool stopped;
    bool paused;
    bool initialized;

    CComPtr<ISpVoice> cpVoice;
    int textLength;
};

QtSpeechWin::QtSpeechWin(QObject * parent)
:QObject(parent)
{
    d = new Private;
    d->paused = false;
    d->stopped = true;
    d->initialized = false;
    create();
}

QtSpeechWin::~QtSpeechWin()
{
    dispose();
    //delete d;
}

bool QtSpeechWin::isValid() const
{
    return d->initialized;
}

bool QtSpeechWin::isActive() const
{
    return !d->stopped;
}

bool QtSpeechWin::isPaused() const
{
    return d->paused;
}

QVariantMap QtSpeechWin::voices()
{
    QVariantMap vs;
        
    HRESULT                             hr = S_OK;
    CComPtr<ISpObjectToken>             cpVoiceToken;
    CComPtr<IEnumSpObjectTokens>        cpEnum;
    ULONG                               ulCount = 0;

    //Enumerate the available voices 
    if(SUCCEEDED(hr))
        hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

    //Get the number of voices
    if(SUCCEEDED(hr))
        hr = cpEnum->GetCount(&ulCount);

    // Obtain a list of available voice tokens, set the voice to the token, and call Speak
    while (SUCCEEDED(hr) && ulCount -- )
    {
        cpVoiceToken.Release();
        if(SUCCEEDED(hr))
            hr = cpEnum->Next( 1, &cpVoiceToken, NULL );

        if (SUCCEEDED(hr)) {
            WCHAR *wDescription = 0L;
            SpGetDescription(cpVoiceToken, &wDescription);

            WCHAR *wID = 0L;
            cpVoiceToken->GetId(&wID);

			QString name = QString::fromWCharArray(wDescription);
			QString voiceID = QString::fromWCharArray(wID);

			vs[name] = voiceID;
        }
    }

    return vs;
}

QVariant QtSpeechWin::defaultVoiceId()
{
	HRESULT                 hr = S_OK;
	CComPtr<ISpVoice>		cpVoice;
    CComPtr<ISpObjectToken>	cpVoiceToken;

    hr = cpVoice.CoCreateInstance( CLSID_SpVoice );
    if (!SUCCEEDED(hr)) 
        return QVariant(); 

    WCHAR *wID = 0L;
    hr = cpVoice->GetVoice(&cpVoiceToken);
    if (!SUCCEEDED(hr))
        return QVariant();

    hr = cpVoiceToken->GetId(&wID);
    if (!SUCCEEDED(hr))
        return QVariant();
    
	return QString::fromWCharArray(wID);
}

void QtSpeechWin::setVoice(QVariant id)
{
    HRESULT                             hr = S_OK;
    CComPtr<ISpObjectToken>             cpVoiceToken;
    CComPtr<IEnumSpObjectTokens>        cpEnum;
    ULONG                               ulCount = 0;

    //Enumerate the available voices 
    if(SUCCEEDED(hr))
        hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

    //Get the number of voices
    if(SUCCEEDED(hr))
        hr = cpEnum->GetCount(&ulCount);

    // Obtain a list of available voice tokens, set the voice to the token, and call Speak
    while (SUCCEEDED(hr) && ulCount -- )
    {
        cpVoiceToken.Release();
        if(SUCCEEDED(hr))
            hr = cpEnum->Next( 1, &cpVoiceToken, NULL );

        if (SUCCEEDED(hr)) {
            WCHAR *wID = 0L;
            cpVoiceToken->GetId(&wID);
            QString voiceID = QString::fromWCharArray(wID);

            if (voiceID == id.toString()) {
                d->cpVoice->SetVoice(cpVoiceToken);
            }
        }
    }
}

void QtSpeechWin::create(QVariant voiceId)
{
    if (d->initialized) 
        return;
    
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
    d->initialized = true;   

	if (voiceId.isValid())
		setVoice(voiceId);
}

void QtSpeechWin::dispose()
{
    d->initialized = false;
}

void QtSpeechWin::start(QString text)
{
	stop();
    metaObject()->invokeMethod(this, "startDelayed", 
                                Qt::QueuedConnection,
                                Q_ARG(QString, text));
}

void QtSpeechWin::startDelayed(QString text)
{
	WCHAR * wcText = new WCHAR[ text.length()+1 ];
    text.toWCharArray(wcText);
    wcText[text.length()] = 0;
    d->textLength = text.length();

    HRESULT hr = d->cpVoice->Speak( wcText, SPF_ASYNC | SPF_IS_NOT_XML, 0 );
    if (FAILED( hr )) qDebug() << "sapi51: Speak() is not completed";
    delete[] wcText;

    d->stopped = false;
}

void QtSpeechWin::stop()
{
    d->stopped = true;

    HRESULT hr = d->cpVoice->Speak( NULL, SPF_PURGEBEFORESPEAK, 0 );
    if (FAILED( hr )) qDebug() << "sapi51: stop is not completed";
}

void QtSpeechWin::pauseSpeech()
{
    if (d->stopped) return;
    if (d->paused) return;
    
    HRESULT hr = d->cpVoice->Pause();
    if (FAILED( hr )) qDebug() << "sapi51: pause is not completed";
    
    d->paused = true;
}

void QtSpeechWin::continueSpeech()
{
    if (d->stopped) return;
    if (!d->paused) return;
    
    HRESULT hr = d->cpVoice->Resume();
    if (FAILED( hr )) qDebug() << "sapi51: resume is not completed";
    
    d->paused = false;    
}

void QtSpeechWin::speechDoneEvent()
{
    if (d->stopped) return;
    emit speechDone();
}

void QtSpeechWin::speechWordEvent(int pos, int length)
{
    emit speechWord(pos, length);
}

void QtSpeechWin::processSAPIEvent()
{
    SPEVENT eventItem;
    memset( &eventItem, 0,sizeof(SPEVENT));
    while( d->cpVoice->GetEvents(1, &eventItem, NULL ) == S_OK )
    {
        switch(eventItem.eEventId )
        {
        case SPEI_WORD_BOUNDARY :
            SPVOICESTATUS eventStatus;
            d->cpVoice->GetStatus( &eventStatus, NULL );

            int wordPos, wordLen;
            wordPos = eventStatus.ulInputWordPos;
            wordLen = eventStatus.ulInputWordLen;
            speechWordEvent(wordPos, wordLen);
            break;

        case SPEI_END_INPUT_STREAM:
			speechWordEvent(d->textLength, 0);
			speechDoneEvent();
            break;

        default:
            break;
        }

        SpClearEvent( &eventItem );
    }
}

} // namespace QtSpeech1

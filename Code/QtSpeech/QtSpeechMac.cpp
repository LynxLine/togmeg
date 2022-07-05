//
// Author Oleksandr Iakovliev <yshurik@gmail.com>
//

#include <QtGui>
#include "QtSpeechMac.h"
#include <ApplicationServices/ApplicationServices.h>

namespace QtSpeech1 { // API v1.0

#define Where QString("%1:%2:").arg(__FILE__).arg(__LINE__)
#define SysCall(x,e) {\
    OSErr ok = x;\
    if (ok != noErr) {\
        QString msg = #e;\
        msg += ":"+QString(__FILE__);\
        msg += ":"+QString::number(__LINE__)+":"+#x;\
        throw e(msg);\
    }\
}

#ifdef Q_OS_MAC64
#define SpeechDoneUPP_ARG2 void *
#else
#define SpeechDoneUPP_ARG2 long
#endif

class QtSpeechMac::Private {
public:
    bool stopped;
    bool paused;
    bool initialized;
    
    SInt16 textScript;
    SpeechChannel speechChannel;
    int currentVoiceId;
    void * SpeechDoneCall;
    void * SpeechWordCall;
    
    static QList<QtSpeechMac *> registered;
    static void SpeechDoneProc(SpeechChannel chan, SpeechDoneUPP_ARG2);
    static void SpeechWordProc(SpeechChannel chan,
                               SpeechDoneUPP_ARG2,
                               unsigned long wordPos,
                               unsigned short wordLen);
};

QList<QtSpeechMac *> QtSpeechMac::Private::registered = QList<QtSpeechMac *>();

QtSpeechMac::QtSpeechMac(QObject * parent)
:QtSpeech(parent)
{
    d = new Private;
    d->paused = false;
    d->stopped = true;
    d->registered << this;
    d->initialized = false;
    d->textScript = kCFStringEncodingMacRoman;
    create();
}

QtSpeechMac::~QtSpeechMac()
{
    d->registered.removeAll(this);
    dispose();
    delete d;
}

bool QtSpeechMac::isValid() const { return d->initialized; }
bool QtSpeechMac::isActive() const { return !d->stopped; }
bool QtSpeechMac::isPaused() const { return d->paused; }

QVariantMap QtSpeechMac::voices() const
{
    QVariantMap vs;
    SInt16 numVoices;
    SysCall( CountVoices(&numVoices), LogicError);
    
    VoiceDescription defaultInfo;
    SysCall( GetVoiceDescription(NULL, &defaultInfo, sizeof(VoiceDescription)), LogicError);
    
    for (int i=1; i<= numVoices; i++) {
        VoiceSpec voice;
        SysCall( GetIndVoice(i, &voice), LogicError);
        
        VoiceDescription info;
        SysCall( GetVoiceDescription(&voice, &info, sizeof(VoiceDescription)), LogicError);
        
        QString name = QString::fromAscii((const char *)(info.name+1), int(info.name[0]));
        vs[name] = int(voice.id);
    }
    
    return vs;
}

QVariant QtSpeechMac::defaultVoiceId() const
{
    VoiceDescription defaultInfo;
    SysCall( GetVoiceDescription(NULL, &defaultInfo, sizeof(VoiceDescription)), LogicError);
    return int(defaultInfo.voice.id);
}

QVariant QtSpeechMac::voiceId() const
{
    if (!d->initialized)
        throw InitError(Where+"Not initialized");

    return d->currentVoiceId;
}

QString QtSpeechMac::voiceName() const
{
    if (!d->initialized)
        throw InitError(Where+"Not initialized");

    SInt16 numVoices;
    SysCall( CountVoices(&numVoices), LogicError);

    VoiceSpec voice;
    for (int i=1; i<= numVoices; i++) {
        SysCall( GetIndVoice(i, &voice), LogicError);
        if (int(voice.id) == d->currentVoiceId) {
            VoiceDescription info;
            SysCall( GetVoiceDescription(&voice, &info, sizeof(VoiceDescription)), LogicError);
            QString name = QString::fromAscii((const char *)(info.name+1), int(info.name[0]));
            return name;
        }
    }

    return QString::null;
}

void QtSpeechMac::setVoice(QVariant id)
{
    dispose();
    create(id);
}

void QtSpeechMac::create(QVariant voiceId)
{
    if (d->initialized) 
        throw InitError(Where+"Not initialized");
    
    SInt16 numVoices;
    SysCall( CountVoices(&numVoices), LogicError);
    
    if (!voiceId.isValid()) {
        VoiceDescription defaultInfo;
        SysCall( GetVoiceDescription(NULL, &defaultInfo, sizeof(VoiceDescription)), LogicError);
        voiceId = int(defaultInfo.voice.id);
    }
    
    VoiceSpec voice;
    VoiceSpec * p_voice = 0L;
    for (int i=1; i<= numVoices; i++) {
        SysCall( GetIndVoice(i, &voice), LogicError);
        if (int(voice.id) == voiceId.toInt()) {
            p_voice = &voice;
            VoiceDescription info;
            SysCall( GetVoiceDescription(&voice, &info, sizeof(VoiceDescription)), LogicError);
            d->textScript = info.script;
            break;
        }
    }    
    
    d->SpeechDoneCall = (void *)NewSpeechDoneUPP( &Private::SpeechDoneProc );
    d->SpeechWordCall = (void *)NewSpeechWordUPP( &Private::SpeechWordProc );

    SysCall( NewSpeechChannel(p_voice,
                              &d->speechChannel
                             ), InitError);
    
    SysCall( SetSpeechInfo(d->speechChannel,
                           soSpeechDoneCallBack,
                           d->SpeechDoneCall
                          ), InitError);
    
    SysCall( SetSpeechInfo(d->speechChannel,
                           soWordCallBack,
                           d->SpeechWordCall
                          ), InitError);
    
    d->initialized = true;
    d->currentVoiceId = voiceId.toInt();
}

void QtSpeechMac::dispose()
{
    if (!d->speechChannel)
        throw CloseError(Where+"No Speech Channel available");
    
    d->initialized = false;
    
    SysCall( StopSpeech(d->speechChannel), CloseError);
    SysCall( DisposeSpeechChannel(d->speechChannel), CloseError);
    
    DisposeSpeechDoneUPP((SpeechDoneUPP)d->SpeechDoneCall);
    DisposeSpeechWordUPP((SpeechWordUPP)d->SpeechWordCall);
    
    d->speechChannel = NULL;
}

void QtSpeechMac::say(QString text)
{
    stop(); 
    CFStringRef cf_text = CFStringCreateWithCharacters(0,
                            reinterpret_cast<const UniChar *>(
                              text.unicode()), text.length());

    OSErr ok = SpeakCFString(d->speechChannel, cf_text, NULL);
    CFRelease(cf_text);

    if (ok != noErr) throw LogicError(Where+"SpeakCFString()");

    d->stopped = false;    
    d->paused = false;
}

void QtSpeechMac::stop()
{
    d->stopped = true;
    d->paused = false;
    
    OSErr ok = StopSpeech(d->speechChannel);
    if (ok != noErr) throw LogicError("StopSpeech()");
    
    ok = SetSpeechInfo(d->speechChannel, soReset, NULL);
    if (ok != noErr) throw LogicError("SetSpeechInfo(), soReset");
    
    SpeechStatusInfo info;
    ok = GetSpeechInfo(d->speechChannel, soStatus, &info);
    if (ok != noErr) throw LogicError("GetSpeechInfo(), soStatus");
}

void QtSpeechMac::pause()
{
    if (d->stopped) return;
    if (d->paused) return;
    
    OSErr ok = PauseSpeechAt(d->speechChannel, kEndOfWord);
    if (ok != noErr) throw LogicError("PauseSpeechAt(), kEndOfWord");
    
    d->paused = true;
}

void QtSpeechMac::resume()
{
    if (d->stopped) return;
    if (!d->paused) return;
    
    OSErr ok = ContinueSpeech(d->speechChannel);
    if (ok != noErr) throw LogicError("ContinueSpeech()");

    d->paused = false;    
}

void QtSpeechMac::Private::SpeechDoneProc(SpeechChannel chan, SpeechDoneUPP_ARG2 refCon)
{
    Q_UNUSED(refCon);
    foreach(QtSpeechMac * channel, registered) {
        if (channel->d->speechChannel == chan) {
            channel->speechDoneEvent();
            break;
        }
    }
}

void QtSpeechMac::Private::SpeechWordProc(SpeechChannel chan,
                                          SpeechDoneUPP_ARG2 refCon,
                                          unsigned long wordPos,
                                          unsigned short wordLen)
{
    Q_UNUSED(refCon);
    foreach(QtSpeechMac * channel, registered) {
        if (channel->d->speechChannel == chan) {
            channel->speechWordEvent(int(wordPos), int(wordLen));
            break;
        }
    }
}

void QtSpeechMac::speechDoneEvent()
{
    if (d->stopped) return;
    emit speechDone();
}

void QtSpeechMac::speechWordEvent(int pos, int length)
{
    emit speechWord(pos, length);
}

} // namespace QtSpeech1

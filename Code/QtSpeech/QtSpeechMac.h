//
// Author Oleksandr Iakovliev <yshurik@gmail.com>
//

#ifndef QtSpeechMac_H
#define QtSpeechMac_H

#include "QtSpeech.h"

namespace QtSpeech1 { // API v1.0

class QtSpeechMac : public QtSpeech {
Q_OBJECT
public:
    virtual ~QtSpeechMac();
    
    bool isValid() const;
    bool isActive() const;
    bool isPaused() const;
    
    QVariant voiceId() const;
    QString voiceName() const;
    void setVoice(QVariant id);
    
    QVariantMap voices() const;
    QVariant defaultVoiceId() const;

public slots:
    void say(QString);
    void stop();
    void pause();
    void resume();

protected:
    void create(QVariant voiceId = QVariant());
    void dispose();

    void speechDoneEvent();
    void speechWordEvent(int pos, int length);
    
private:
    friend QtSpeech * QtSpeech::Create(QObject *);
    QtSpeechMac(QObject *);

    class Private;
    Private * d;
};

} // namespace QtSpeech1
#endif // QtSpeechMac_H

//
// Author Oleksandr Iakovliev <yshurik@gmail.com>
//

#ifndef QtSpeechWin_H
#define QtSpeechWin_H

#include "QtSpeech.h"

namespace QtSpeech1 { // API v1.0

class QtSpeechWin : public QtSpeech {
Q_OBJECT
public:
    virtual ~QtSpeechWin();
    
    bool isValid() const;
    bool isActive() const;
    bool isPaused() const;
    
    void setVoice(QVariant id);
    
    void say(QString);
    void stop();
    
    void pause();
    void resume();

    static QVariantMap voices();
    static QVariant defaultVoiceId();

protected:
    void create(QVariant voiceId = QVariant());
    void dispose();

	void processSAPIEvent();
    void speechDoneEvent();
    void speechWordEvent(int pos, int length);
    
private slots:
    void startDelayed(QString);

private:
    friend QtSpeech * QtSpeech::Create(QObject *);
    QtSpeechWin(QObject *);

    class Private;
    Private * d;
};

} // namespace QtSpeech1
#endif // QtSpeechWin_H

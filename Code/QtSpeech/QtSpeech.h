//
// Author Oleksandr Iakovliev <yshurik@gmail.com>
//

#ifndef QtSpeech_H
#define QtSpeech_H

#include <QObject>
#include <QVariant>

namespace QtSpeech1 { // API v1.0

class QtSpeech : public QObject {
Q_OBJECT
public:
    class Error {
    public:
        Error(QString s):msg(s) {}
        QString msg;
    };

    class InitError : public Error { public: InitError(QString s):Error(s) {} };
    class LogicError : public Error { public: LogicError(QString s):Error(s) {} };
    class CloseError : public Error { public: CloseError(QString s):Error(s) {} };

    static QtSpeech * Create(QObject * parent);
    virtual ~QtSpeech() {}

    virtual bool isValid() const =0;
    virtual bool isActive() const =0;
    virtual bool isPaused() const =0;

    virtual QVariant voiceId() const =0;
    virtual QString voiceName() const =0;
    virtual void setVoice(QVariant id) =0;

    virtual void say(QString) =0;
    virtual void stop() =0;
    virtual void pause() =0;
    virtual void resume() =0;

    virtual QVariantMap voices() const =0;
    virtual QVariant defaultVoiceId() const =0;

signals:
    void speechDone();
    void speechWord(int pos, int length);

protected:
    QtSpeech(QObject * p):QObject(p) {}
};

} // namespace QtSpeech1
#endif // QtSpeech_H

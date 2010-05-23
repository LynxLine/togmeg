//
// Copyright (C) 2007 CafeScribe
//

#ifndef CRAMMERO_H
#define CRAMMERO_H

#include <QString>
#include <QApplication>
#include <QWindowsStyle>

#define CB_TIME_SECONDS 0
#define CB_TIME_MINUTES 1

namespace app {

    extern QWindowsStyle cleanStyle;

    //typos in minute
    int typingSpeed();
    void addTypingStats(int msecs, int count);

}; //namespace

class Application : public QApplication {
Q_OBJECT
public:
    Application(int argc, char **argv);
    virtual ~Application();

    QString buildId() const;

signals:
    //! Emits when application gets QEvent::FileOpen event
    void fileOpenRequest(QString);
    
protected:
    virtual bool event(QEvent *);
    
private:
    class Private;
    Private * d;
};

#endif // CRAMMERO_H

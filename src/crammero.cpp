//
// Copyright (C) 2007 LynxLine
//

#include <QtGui>
#include <time.h>
#include "crammero.h"
using namespace app;

QWindowsStyle app::cleanStyle;

int app::typingSpeed()
{
    QSettings s;
    int keySpeed = s.value("stats/keySpeed", 60).toInt();
    return keySpeed;
}

void app::addTypingStats(int msecs, int count)
{
    QSettings s;
    int keySpeed = s.value("stats/keySpeed", 60).toInt();
    qlonglong keyStatsTime = s.value("stats/keyStatsTime", 60000).toLongLong();
    qlonglong keyCount = qlonglong(keySpeed) * keyStatsTime /(60000);
    
    keyStatsTime += qlonglong(msecs);
    keyCount += qlonglong(count);

    //to be sure
    if (keyStatsTime <=0 ) keyStatsTime = 60000;

    keySpeed = int( (keyCount * 60000)/keyStatsTime );

    s.setValue("stats/keyStatsTime", keyStatsTime);
    s.setValue("stats/keySpeed", keySpeed);
    s.sync();
}

//
// Application
//

class Application::Private {
public:
};

Application::Application(int argc, char **argv)
:QApplication(argc, argv)
{
    d = new Private;
}

Application::~Application()
{
    delete d;
}

/*!
 Reimplemented from QApplication to handle
 application-like event, e.g. QEvent::FileOpen
 */
bool Application::event(QEvent * event)
{
    switch (event->type()) {
        case QEvent::FileOpen: {
            QFileOpenEvent * foe = (QFileOpenEvent *)event;
            emit fileOpenRequest( foe->file() );
            return true;
        }
        default:
            return QApplication::event(event);
    }
    
    return false;
}

QString Application::buildId() const
{
    QString embeddedDate(__DATE__);
    QString m = embeddedDate.left(3);
    QStringList months;
    months << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun"
    << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    
    int month = months.indexOf(m) + 1;
    int day   = embeddedDate.mid(4, 2).trimmed().toInt();
    int year  = embeddedDate.mid(7, 4).trimmed().toInt();
    
    return QString::number(year*10000+month*100+day);
}

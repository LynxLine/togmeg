//
// Copyright (C) 2007 CafeScribe
//

#include <time.h>
#include <QtCore>
#include "crammero.h"
using namespace app;

QWindowsStyle app::cleanStyle;

QString app::storagePath() {
    QString sep = QDir::separator();
    QString _storagePath = QDir::homePath();
    _storagePath = _storagePath.replace('/', QDir::separator());

#ifdef Q_WS_WIN
    _storagePath += sep +"Application Data" +sep +"Crammero" +sep;
#else
    _storagePath += sep +QString(".crammero") +sep;
#endif
    return _storagePath;
}

QString app::uniqueId(int length)
{
    static bool initialized = false;
    if ( !initialized ) {
        initialized = true;
        srand(time(NULL));
    }

    QString id;
    QStringList slist;
    slist << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7"
          << "8" << "9" << "a" << "b" << "c" << "d" << "e" << "f";
    for (int i=0;i<length;i++) {
        id += slist[ rand() % 16 ];
    }

    return id;
}

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

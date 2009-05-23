//
// Copyright (C) 2007 CafeScribe
//

#include <time.h>
#include <QtCore>
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

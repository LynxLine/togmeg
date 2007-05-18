//
// Copyright (C) 2007 CafeScribe
//

#ifndef CRAMMERO_H
#define CRAMMERO_H

#include <QString>
#include <QWindowsStyle>

#define CB_TIME_SECONDS 0
#define CB_TIME_MINUTES 1

namespace app {

    QString storagePath();
    QString uniqueId(int length = 10);
    extern QWindowsStyle cleanStyle;

    //typos in minute
    int typingSpeed();
    void addTypingStats(int msecs, int count);

}; //namespace

#endif // CRAMMERO_H

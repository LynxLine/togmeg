//
// Copyright (C) 2007 CafeScribe
//

#include <time.h>
#include <QtCore>
#include "crammero.h"
using namespace app;

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
    srand(time(NULL));

    QString id;
    QStringList slist;
    slist << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7"
          << "8" << "9" << "a" << "b" << "c" << "d" << "e" << "f";
    for (int i=0;i<length;i++) {
        id += slist[ rand() % 16 ];
    }

    return id;
}

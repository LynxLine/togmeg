//
// Copyright (C) 2007 CafeScribe
//

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

//
// Copyright (C) 2007 CafeScribe
//

#ifndef CRAMMERO_H
#define CRAMMERO_H

#include <QString>
#include <QWindowsStyle>

namespace app {

    QString storagePath();
    QString uniqueId(int length = 10);
    extern QWindowsStyle cleanStyle;

}; //namespace

#endif // CRAMMERO_H

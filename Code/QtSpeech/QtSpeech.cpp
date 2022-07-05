//
// Author Oleksandr Iakovliev <yshurik@gmail.com>
//

#include "QtSpeech.h"

#ifdef Q_WS_MACX
    #include "QtSpeechMac.h"
#elif Q_WS_WIN
    #include "QtSpeechWin.h"
#else
    #include "???"
#endif

namespace QtSpeech1 { // API v1.0

QtSpeech * QtSpeech::Create(QObject * parent)
{
#ifdef Q_WS_MACX
    return new QtSpeechMac(parent);
#elif Q_WS_WIN
    return new QtSpeechWin(parent);
#else
    return 0L;
#endif
}

} //namespace QtSpeech1

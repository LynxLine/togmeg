
INCLUDEPATH += $$PWD

HEADERS += $$PWD/QtSpeech.h
SOURCES += $$PWD/QtSpeech.cpp

macx {
    HEADERS += $$PWD/QtSpeechMac.h
    SOURCES += $$PWD/QtSpeechMac.cpp
}

win32 {
    HEADERS += $$PWD/QtSpeechWin.h
    SOURCES += $$PWD/QtSpeechWin.cpp
}

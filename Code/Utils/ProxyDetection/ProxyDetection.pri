
win32 {
    LIBS += -L"$$PWD" winhttp.lib
    INCLUDEPATH += $$PWD
    HEADERS += $$PWD/ProxyDetection.h
    SOURCES += $$PWD/ProxyDetection.cpp
}

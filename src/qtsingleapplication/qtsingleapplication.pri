
DEPENDPATH += qtsingleapplication
INCLUDEPATH	+= qtsingleapplication

HEADERS		+= qtsingleapplication/qtsingleapplication.h
SOURCES		+= qtsingleapplication/qtsingleapplication.cpp

mac:SOURCES       += qtsingleapplication/qtsingleapplication_mac.cpp
win32:SOURCES     += qtsingleapplication/qtsingleapplication_win.cpp
unix:!mac:SOURCES += qtsingleapplication/qtsingleapplication_x11.cpp


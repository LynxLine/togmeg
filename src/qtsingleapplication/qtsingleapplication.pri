
DEPENDPATH += src/qtsingleapplication
INCLUDEPATH	+= src/qtsingleapplication

HEADERS		+= src/qtsingleapplication/qtsingleapplication.h
SOURCES		+= src/qtsingleapplication/qtsingleapplication.cpp

mac:SOURCES       += src/qtsingleapplication/qtsingleapplication_mac.cpp
win32:SOURCES     += src/qtsingleapplication/qtsingleapplication_win.cpp
unix:!mac:SOURCES += src/qtsingleapplication/qtsingleapplication_x11.cpp


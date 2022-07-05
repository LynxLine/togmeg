
INCLUDEPATH += $$PWD
HEADERS += $$PWD/QtLockedFile.h
SOURCES += $$PWD/qtlockedfile.cpp
unix:SOURCES += $$PWD/qtlockedfile_unix.cpp
win32:SOURCES += $$PWD/qtlockedfile_win.cpp


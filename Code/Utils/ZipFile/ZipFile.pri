
INCLUDEPATH += $$PWD 
DEFINES += NOUNCRYPT NOCRYPT

HEADERS += \
    $$PWD/ZipFile.h \

SOURCES += \
    $$PWD/zip.c \
    $$PWD/unzip.c \
    $$PWD/ioapi.c \
    $$PWD/ZipFile.cpp \

win32:SOURCES += $$PWD/iowin32.c

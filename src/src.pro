
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

unix:TARGET = ../bin/aLesson
win32:TARGET = ..\..\bin\aLesson

win32:LIBS += shell32.lib
unix:LIBS += -lz

CONFIG += qt
CONFIG += warn_on
CONFIG += debug_and_release

precompile_header {
    CONFIG += precompile_header
    PRECOMPILED_HEADER = stable.h
    HEADERS += stable.h
    unix:QMAKE_CXXFLAGS += -fpch-preprocess
}

QT += xml
QT += sql
QT += network

include('study/study.pri')
include('testproperties/testproperties.pri')

HEADERS += \
    boxwindow.h \
    mainwindow.h \
    movingwidget.h \
    timeoutpanel.h \

SOURCES += \
    main.cpp \
    boxwindow.cpp \
    mainwindow.cpp \
    movingwidget.cpp \
    timeoutpanel.cpp \

RESOURCES += images.qrc

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)_debug
        CONFIG += console
        UI_DIR = debug
    }
    else {
        UI_DIR = release
        unix:QMAKE_CXXFLAGS += -g
    }
}

win32:RC_FILE = images/alesson.rc

# uncomment this to build with profile info:
# QMAKE_CXXFLAGS += -g -pg
# QMAKE_LDFLAGS += -g -pg


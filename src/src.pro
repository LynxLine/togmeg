
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

unix:!mac:TARGET = ../bin/serrater
win32:TARGET = ..\..\bin\serrater
mac:TARGET = ../bin/serrater

win32:LIBS += user32.lib
win32:LIBS += shell32.lib
win32:LIBS += winhttp.lib

mac:LIBS += -framework CoreFoundation
mac:LIBS += -framework IOKit
mac:QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
mac:CONFIG += x86 ppc

unix:LIBS += -lz
unix:!mac:CONFIG += x11

CONFIG += qt
CONFIG += warn_on
CONFIG += debug_and_release

unix:CONFIG += precompile_header

!mac:precompile_header {
    PRECOMPILED_HEADER = stable.h
    HEADERS += stable.h
    unix:QMAKE_CXXFLAGS += -fpch-preprocess
}

QT += xml
QT += svg
QT += opengl
QT += network


INCLUDEPATH += .

HEADERS += \
    serrater.h \
    mainwindow.h \


SOURCES += \
    main.cpp \
    serrater.cpp \
    mainwindow.cpp \


include("svg/svg.pri")
include("logger/logger.pri")
include("storage/storage.pri")
include("catalog/catalog.pri")
include("examine/examine.pri")
include("qtsingleapplication/qtsingleapplication.pri")

RESOURCES += images.qrc

mac:QMAKE_INFO_PLIST = info.plist

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)_debug
        UI_DIR = debug
        CONFIG += console
    }
    else {
        UI_DIR = release
        unix:QMAKE_CXXFLAGS += -g
    }
}

win32:RC_FILE = images/serrater.rc
mac:ICON = images/serrater.icns

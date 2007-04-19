
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

unix:!mac:TARGET = ../bin/crammero
win32:TARGET = ..\..\bin\crammero
mac:TARGET = ../bin/crammero

win32:LIBS += user32.lib
win32:LIBS += shell32.lib
win32:LIBS += winhttp.lib

CONFIG += qt
CONFIG += warn_on
CONFIG += debug_and_release

unix:CONFIG += precompile_header

QT += xml
QT += svg
QT += opengl
QT += network

INCLUDEPATH += .

HEADERS += \
    crammero.h \
    mainwindow.h \
    studytask.h \


SOURCES += \
    main.cpp \
    crammero.cpp \
    mainwindow.cpp \
    studytask.cpp \


include("svg/svg.pri")
include("logger/logger.pri")
include("storage/storage.pri")
include("catalog/catalog.pri")
include("examine/examine.pri")
include("widgets/widgets.pri")
include("taskeditor/taskeditor.pri")
include("qtsingleapplication/qtsingleapplication.pri")

RESOURCES += images.qrc

mac:QMAKE_INFO_PLIST = info.plist

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)_debug
        UI_DIR = debug
        CONFIG += console
        
        HEADERS += stable.h
        PRECOMPILED_HEADER = stable.h
        unix:QMAKE_CXXFLAGS += -fpch-preprocess
    }
    else {
        UI_DIR = release

        mac:CONFIG += x86 ppc
        mac:QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    }
}

win32:RC_FILE = images/crammero.rc
mac:ICON = images/crammero.icns

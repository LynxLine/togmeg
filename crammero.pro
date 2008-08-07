
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

#unix:CONFIG += precompile_header

QT += xml

INCLUDEPATH += .

HEADERS += \
    src/crammero.h \
    src/mainwindow.h \
    src/studytask.h \


SOURCES += \
    src/main.cpp \
    src/crammero.cpp \
    src/mainwindow.cpp \
    src/studytask.cpp \

UI_DIR = ./build
MOC_DIR = ./build

include("src/logger/logger.pri")
include("src/storage/storage.pri")
include("src/catalog/catalog.pri")
include("src/examine/examine.pri")
include("src/widgets/widgets.pri")
include("src/taskeditor/taskeditor.pri")
include("src/qtsingleapplication/qtsingleapplication.pri")

RESOURCES += images.qrc

mac:QMAKE_INFO_PLIST = info.plist
mac:CONFIG += x86 ppc
mac:QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk

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
    }
}

win32:RC_FILE = images/crammero.rc
mac:ICON = images/crammero.icns

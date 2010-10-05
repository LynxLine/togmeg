
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

unix:TARGET = TogMeg
win32:TARGET = ../TogMeg

CONFIG += qt
CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += precompile_header

QT += gui xml

HEADERS += Code/stable.h
PRECOMPILED_HEADER = Code/stable.h

UI_DIR = ./build/GeneratedFiles
MOC_DIR = ./build/GeneratedFiles
RCC_DIR = ./build/GeneratedFiles

include(Code/Utils/Utils.pri)
include(Code/TogMeg/TogMeg.pri)

RESOURCES += images.qrc

win32 {
    RC_FILE = Resources/TogMeg.rc
    DEFINES += _CRT_SECURE_NO_DEPRECATE
    DEFINES += _CRT_NONSTDC_NO_DEPRECATE
}

#!debug_and_release|build_pass {
#    CONFIG(debug, debug|release) {
#        TARGET = $$member(TARGET, 0)d
#        CONFIG += console
#    }
#}

macx {
    ICON = Resources/TogMeg.icns
    LIBS += -framework CoreFoundation -lz
    LIBS += -framework AppKit -framework Carbon
    QMAKE_INFO_PLIST = Resources/TogMeg.plist
}

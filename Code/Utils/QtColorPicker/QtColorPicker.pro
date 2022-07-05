
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

TARGET = QtColorPicker

UI_DIR = ./build
MOC_DIR = ./build

QT += gui
CONFIG += qt
CONFIG -= flat
CONFIG += warn_on
CONFIG += debug_and_release

CONFIG += coloredit

SOURCES += Main.cpp
include(QtColorPicker.pri)

win32 {
    DEFINES += _CRT_SECURE_NO_DEPRECATE
    DEFINES += _CRT_NONSTDC_NO_DEPRECATE
    !debug_and_release|build_pass {
        CONFIG(debug, debug|release) {
            CONFIG += console        
        }
    }
}

macx {
    LIBS += -framework CoreFoundation -lz
    LIBS += -framework AppKit -framework Carbon
    QMAKE_CXXFLAGS *= -fvisibility=hidden -fvisibility-inlines-hidden
}

macx-xcode {
    QMAKE_CC = gcc
    QMAKE_CXX = g++
    QMAKE_CFLAGS_DEBUG =
    QMAKE_CFLAGS_RELEASE =
    QMAKE_CXXFLAGS_DEBUG =
    QMAKE_CXXFLAGS_RELEASE =
}


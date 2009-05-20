
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

unix:TARGET = crammero
win32:TARGET = ../crammero

CONFIG += qt
CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += precompile_header

HEADERS += src/stable.h
PRECOMPILED_HEADER = src/stable.h

QT += xml

INCLUDEPATH += src
INCLUDEPATH += src/Utils

HEADERS += \
    src/crammero.h \
    src/mainwindow.h \
    src/studytask.h \
    src/Utils/AppStyles.h \
    src/Utils/PixmapButton.h \
    src/Utils/ColumnSelectorButton.h \

SOURCES += \
    src/main.cpp \
    src/crammero.cpp \
    src/mainwindow.cpp \
    src/studytask.cpp \
    src/Utils/AppStyles.cpp \
    src/Utils/PixmapButton.cpp \
    src/Utils/ColumnSelectorButton.cpp \

UI_DIR = ./build/GeneratedFiles
MOC_DIR = ./build/GeneratedFiles

include("src/storage/storage.pri")
include("src/examine/examine.pri")
include("src/widgets/widgets.pri")
include("src/taskeditor/taskeditor.pri")

RESOURCES += images.qrc

mac:ICON = images/crammero.icns
mac:QMAKE_INFO_PLIST = info.plist

win32:RC_FILE = images/crammero.rc

!debug_and_release|build_pass {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
        CONFIG += console
    }
}

macx-g++ {
    # section for batch building
    # from command line on mac osx.
    
    CONFIG += x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.4
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    
    !debug_and_release|build_pass {
        CONFIG(release, debug|release) {
            # release build from console: we build application
            # bundle with everything embedded into this bundle
            # it builds universal binaries, so pch disabled
            # use QMAKESPEC=macx-g++ for such build
            
            CONFIG += x86 ppc
            CONFIG -= precompile_header

            TARGET = "Crammero"
            BUNDLETARGET = "Crammero.app"

            QMAKE_PRE_LINK = \
                rm -rf $${BUNDLETARGET}/Contents/plugins; \
                rm -rf $${BUNDLETARGET}/Contents/Frameworks
            
            QMAKE_POST_LINK = \
                mkdir -p $${BUNDLETARGET}/Contents/Frameworks; \
                cp -R /Library/Frameworks/QtCore.framework \
                      /Library/Frameworks/QtGui.framework \
                      /Library/Frameworks/QtXml.framework \
                      $${BUNDLETARGET}/Contents/Frameworks; \
                \
                install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/Frameworks/QtCore.framework/Versions/4/QtCore; \
                install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui; \
                install_name_tool -id @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml \
                                $${BUNDLETARGET}/Contents/Frameworks/QtXml.framework/Versions/4/QtXml; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $(TARGET); \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $(TARGET); \
                install_name_tool -change QtXml.framework/Versions/4/QtXml \
                                @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml $(TARGET); \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui; \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/Frameworks/QtXml.framework/Versions/4/QtXml; \
                \
                otool -L $(TARGET); \
                mkdir -p $${BUNDLETARGET}/Contents/plugins; \
                cp -R /Developer/Applications/Qt/plugins/imageformats $${BUNDLETARGET}/Contents/plugins; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqjpeg.dylib; \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqjpeg.dylib; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqgif.dylib; \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqgif.dylib; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqmng.dylib; \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqmng.dylib; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqsvg.dylib; \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqsvg.dylib; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqtiff.dylib; \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqtiff.dylib; \
                \
                install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqico.dylib; \
                install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/plugins/imageformats/libqico.dylib; \
                \
                echo "Ok"
        }
        else {
            # debug batch build
            TARGET = "crammerod"
        }
    }
}

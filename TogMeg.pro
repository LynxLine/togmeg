
unix:TEMPLATE = app
win32:TEMPLATE = vcapp

CONFIG += qt
CONFIG += warn_on
CONFIG += debug_and_release

QT += gui xml
CONFIG += speech

HEADERS += Code/stable.h
CONFIG += precompile_header
PRECOMPILED_HEADER = Code/stable.h

UI_DIR = ./build/GeneratedFiles
MOC_DIR = ./build/GeneratedFiles
RCC_DIR = ./build/GeneratedFiles

include(Code/Utils/Utils.pri)
include(Code/TogMeg/TogMeg.pri)
include(Code/QtSpeech/QtSpeech.pri)

RESOURCES += Graphics/Icons.qrc
RESOURCES += Graphics/Images.qrc

win32 {
    RC_FILE = Resources/TogMeg.rc
    DEFINES += _CRT_SECURE_NO_DEPRECATE
    DEFINES += _CRT_NONSTDC_NO_DEPRECATE
}

macx {
    CONFIG += x86
    CONFIG -= x86_64
    CONFIG -= ppc
    QMAKE_LFLAGS += -m32
    QMAKE_CXXFLAGS += -m32

    ICON = Resources/TogMeg.icns
    LIBS += -framework CoreFoundation -lz
    LIBS += -framework AppKit -framework Carbon
    QMAKE_INFO_PLIST = Resources/TogMeg.plist

    BUNDLE_EXAMPLES.path = Contents/Resources/Examples
    BUNDLE_EXAMPLES.files = \
        "Resources/Examples/Study Norwegian" \

    QMAKE_BUNDLE_DATA += \
        BUNDLE_EXAMPLES \
}

macx-g++ {
    # section for batch building
    # from command line on mac osx.
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.5
    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk

    !debug_and_release|build_pass {
        CONFIG(release, debug|release) {
            # release build from console: we build application
            # bundle with everything embedded into this bundle
            # it builds universal binaries, so pch is disabled
            # use QMAKESPEC=macx-g++ for such build

            CONFIG -= precompile_header

            TARGET = TogMeg
            BUNDLETARGET = TogMeg.app
            QTFRAMEWORKSPATH = /usr/local/Trolltech/Qt-4.7.3/lib
            QTFRAMEWORKSREF = /usr/local/Trolltech/Qt-4.7.3/lib/

            QMAKE_PRE_LINK = \
                rm -rf $${BUNDLETARGET}/Contents/PlugIns; \
                rm -rf $${BUNDLETARGET}/Contents/Frameworks

            QMAKE_POST_LINK = \
                mkdir -p $${BUNDLETARGET}/Contents/Frameworks; \
                cp -R $${QTFRAMEWORKSPATH}/QtCore.framework \
                      $${QTFRAMEWORKSPATH}/QtGui.framework \
                      $${QTFRAMEWORKSPATH}/QtXml.framework \
                      $${BUNDLETARGET}/Contents/Frameworks; \
                \
                install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/Frameworks/QtCore.framework/Versions/4/QtCore; \
                install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                $${BUNDLETARGET}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui; \
                install_name_tool -id @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml \
                                $${BUNDLETARGET}/Contents/Frameworks/QtXml.framework/Versions/4/QtXml; \
                \
                install_name_tool -change $${QTFRAMEWORKSREF}QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $(TARGET); \
                install_name_tool -change $${QTFRAMEWORKSREF}QtGui.framework/Versions/4/QtGui \
                                @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $(TARGET); \
                install_name_tool -change $${QTFRAMEWORKSREF}QtXml.framework/Versions/4/QtXml \
                                @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml $(TARGET); \
                \
                install_name_tool -change $${QTFRAMEWORKSREF}QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui; \
                install_name_tool -change $${QTFRAMEWORKSREF}QtCore.framework/Versions/4/QtCore \
                                @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                $${BUNDLETARGET}/Contents/Frameworks/QtXml.framework/Versions/4/QtXml; \
                \
                strip $(TARGET); \
                strip -x $${BUNDLETARGET}/Contents/Frameworks/QtCore.framework/Versions/4/QtCore; \
                strip -x $${BUNDLETARGET}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui; \
                strip -x $${BUNDLETARGET}/Contents/Frameworks/QtXml.framework/Versions/4/QtXml; \
                \
                rm -rf `find $${BUNDLETARGET} -name "*.prl"`; \
                rm -rf `find $${BUNDLETARGET} -name "*.lproj"`; \
                rm -rf `find $${BUNDLETARGET} -name "*_debug*"`; \
                rm -rf `find $${BUNDLETARGET} -name "Headers"`; \
                \
                /usr/bin/codesign -f -s 3rd\ Party\ Mac\ Developer\ Application:\ Oleksandr\ Iakovliev $${BUNDLETARGET}; \
                \
                otool -L $(TARGET); \
                otool -L $${BUNDLETARGET}/Contents/Frameworks/QtCore.framework/Versions/4/QtCore; \
                otool -L $${BUNDLETARGET}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui; \
                otool -L $${BUNDLETARGET}/Contents/Frameworks/QtXml.framework/Versions/4/QtXml; \
                \
                echo "Ok"
        }
        else {
            # debug batch build
            TARGET = TogMeg
        }
    }
}


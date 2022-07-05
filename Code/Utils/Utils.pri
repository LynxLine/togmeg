
zipfile:        include($$PWD/ZipFile/ZipFile.pri)
navigation:     include($$PWD/Navigation/Navigation.pri)
proxy:          include($$PWD/ProxyDetection/ProxyDetection.pri)
textedit:       include($$PWD/TextEditDialog/TextEditDialog.pri)
coloredit:      include($$PWD/QtColorPicker/QtColorPicker.pri)
lockedfile:     include($$PWD/QtLockedFile/QtLockedFile.pri)
fileswatcher:   include($$PWD/FilesWatcher/FilesWatcher.pri)
onlineupdates:  include($$PWD/SoftwareUpdate/SoftwareUpdate.pri)

INCLUDEPATH += $$PWD 

HEADERS +=  \
    $$PWD/qgetopt.h \
    $$PWD/rijndael.h \
    $$PWD/XML_Util.h \
    $$PWD/XML_Reader.h \
    $$PWD/XML_Handler.h \
    $$PWD/BaseProject.h \
    $$PWD/MiscUtils.h \

SOURCES +=  \
    $$PWD/qgetopt.cpp \
    $$PWD/rijndael.cpp \
    $$PWD/XML_Util.cpp \
    $$PWD/XML_Reader.cpp \
    $$PWD/BaseProject.cpp \
    $$PWD/MiscUtils.cpp \

contains(QT,gui) {

include($$PWD/PixmapButton/PixmapButton.pri)

HEADERS +=  \
    $$PWD/IconSet.h \
    $$PWD/AppStyles.h \
    $$PWD/HelpButton.h \
    $$PWD/FlowLayout.h \
    $$PWD/FilterLineEdit.h \
    $$PWD/LabelWithProgress.h \
    $$PWD/LineEditWithProgress.h \
    $$PWD/ColumnSelectorButton.h \
    $$PWD/DecoratedItemDelegate.h \
    $$PWD/GroupingProxyModel.h \
    $$PWD/UngroupProxyModel.h \
    $$PWD/ProgressBar.h \
    $$PWD/BaseWindow.h \

SOURCES +=  \
    $$PWD/IconSet.cpp \
    $$PWD/AppStyles.cpp \
    $$PWD/HelpButton.cpp \
    $$PWD/FlowLayout.cpp \
    $$PWD/FilterLineEdit.cpp \
    $$PWD/LabelWithProgress.cpp \
    $$PWD/LineEditWithProgress.cpp \
    $$PWD/ColumnSelectorButton.cpp \
    $$PWD/DecoratedItemDelegate.cpp \
    $$PWD/GroupingProxyModel.cpp \
    $$PWD/UngroupProxyModel.cpp \
    $$PWD/ProgressBar.cpp \
    $$PWD/BaseWindow.cpp \

}

pipeit:exists($$PWD/UtilsEx.pri){
    include($$PWD/UtilsEx.pri)
}

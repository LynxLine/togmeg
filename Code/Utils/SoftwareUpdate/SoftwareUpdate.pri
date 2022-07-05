
INCLUDEPATH += $$PWD 


contains(QT,gui) {
    !contains(QT,network):QT += network

    HEADERS += \
        $$PWD/SoftwareUpdate.h \

    SOURCES += \
        $$PWD/SoftwareUpdate.cpp \

    FORMS += \
        $$PWD/SoftwareUpdate.ui \
        $$PWD/SoftwareUpdateProgress.ui \
}

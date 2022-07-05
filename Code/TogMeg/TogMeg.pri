
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/TogMeg.h \
    $$PWD/TogMegProject.h \
    $$PWD/QAModel.h \
    $$PWD/Examinator.h \
    $$PWD/TaskController.h \
    $$PWD/PlayController.h \
    $$PWD/StudyController.h \

SOURCES += \
    $$PWD/TogMeg.cpp \
    $$PWD/TogMegProject.cpp \
    $$PWD/QAModel.cpp \
    $$PWD/Examinator.cpp \
    $$PWD/TaskController.cpp \
    $$PWD/PlayController.cpp \
    $$PWD/StudyController.cpp \

INCLUDEPATH += $$PWD/Gui

HEADERS += \
    $$PWD/Gui/TogMegWindow.h \
    $$PWD/Gui/FileNavigationView.h \
    $$PWD/Gui/FileNavigationModel.h \

SOURCES += \
    $$PWD/Gui/Main.cpp \
    $$PWD/Gui/TogMegWindow.cpp \
    $$PWD/Gui/FileNavigationView.cpp \
    $$PWD/Gui/FileNavigationModel.cpp \

INCLUDEPATH += $$PWD/Editor

HEADERS += \
    $$PWD/Editor/VHeader.h \
    $$PWD/Editor/QAEditor.h \
    $$PWD/Editor/QADelegate.h \

SOURCES += \
    $$PWD/Editor/VHeader.cpp \
    $$PWD/Editor/QAEditor.cpp \
    $$PWD/Editor/QADelegate.cpp \

INCLUDEPATH += $$PWD/Player

HEADERS += \
    $$PWD/Player/TimeLineWidget.h \
    $$PWD/Player/ExamineWidget.h \
    $$PWD/Player/QuestionWidget.h \
    $$PWD/Player/AnswerWidget.h \
    $$PWD/Player/RoundedLineEdit.h \
    $$PWD/Player/GradientWidget.h \

SOURCES += \
    $$PWD/Player/TimelineWidget.cpp \
    $$PWD/Player/ExamineWidget.cpp \
    $$PWD/Player/QuestionWidget.cpp \
    $$PWD/Player/AnswerWidget.cpp \
    $$PWD/Player/RoundedLineEdit.cpp \
    $$PWD/Player/GradientWidget.cpp \


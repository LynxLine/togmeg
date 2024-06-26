#############################################################################
# Makefile for building: TogMeg.app/Contents/MacOS/TogMeg
# Generated by qmake (2.01a) (Qt 4.7.3) on: Mon Jun 20 00:00:59 2011
# Project:  TogMeg.pro
# Template: app
# Command: /usr/local/Trolltech/Qt-4.7.3/bin/qmake -o TogMeg.xcodeproj/project.pbxproj TogMeg.pro
#############################################################################

MOC       = /usr/local/Trolltech/Qt-4.7.3/bin/moc
UIC       = /usr/local/Trolltech/Qt-4.7.3/bin/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I/usr/local/Trolltech/Qt-4.7.3/mkspecs/macx-xcode -I. -I/usr/local/Trolltech/Qt-4.7.3/lib/QtCore.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.7.3/include/QtCore -I/usr/local/Trolltech/Qt-4.7.3/lib/QtGui.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.7.3/include/QtGui -I/usr/local/Trolltech/Qt-4.7.3/lib/QtXml.framework/Versions/4/Headers -I/usr/local/Trolltech/Qt-4.7.3/include/QtXml -I/usr/local/Trolltech/Qt-4.7.3/include -ICode/Utils -ICode/Utils/PixmapButton -ICode/TogMeg -ICode/TogMeg/Gui -ICode/TogMeg/Editor -ICode/TogMeg/Player -ICode/QtSpeech -Ibuild/GeneratedFiles -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F/usr/local/Trolltech/Qt-4.7.3/lib
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compilers: ./build/GeneratedFiles/moc_BaseProject.cpp ./build/GeneratedFiles/moc_PixmapButton.cpp ./build/GeneratedFiles/moc_HelpButton.cpp\
	 ./build/GeneratedFiles/moc_FilterLineEdit.cpp ./build/GeneratedFiles/moc_LabelWithProgress.cpp ./build/GeneratedFiles/moc_LineEditWithProgress.cpp\
	 ./build/GeneratedFiles/moc_ColumnSelectorButton.cpp ./build/GeneratedFiles/moc_GroupingProxyModel.cpp ./build/GeneratedFiles/moc_UngroupProxyModel.cpp\
	 ./build/GeneratedFiles/moc_ProgressBar.cpp ./build/GeneratedFiles/moc_BaseWindow.cpp ./build/GeneratedFiles/moc_TogMeg.cpp\
	 ./build/GeneratedFiles/moc_TogMegProject.cpp ./build/GeneratedFiles/moc_QAModel.cpp ./build/GeneratedFiles/moc_Examinator.cpp\
	 ./build/GeneratedFiles/moc_TaskController.cpp ./build/GeneratedFiles/moc_PlayController.cpp ./build/GeneratedFiles/moc_StudyController.cpp\
	 ./build/GeneratedFiles/moc_TogMegWindow.cpp ./build/GeneratedFiles/moc_FileNavigationView.cpp ./build/GeneratedFiles/moc_FileNavigationModel.cpp\
	 ./build/GeneratedFiles/moc_VHeader.cpp ./build/GeneratedFiles/moc_QAEditor.cpp ./build/GeneratedFiles/moc_QADelegate.cpp\
	 ./build/GeneratedFiles/moc_TimeLineWidget.cpp ./build/GeneratedFiles/moc_ExamineWidget.cpp ./build/GeneratedFiles/moc_QuestionWidget.cpp\
	 ./build/GeneratedFiles/moc_AnswerWidget.cpp ./build/GeneratedFiles/moc_RoundedLineEdit.cpp ./build/GeneratedFiles/moc_GradientWidget.cpp\
	 ./build/GeneratedFiles/moc_QtSpeech.cpp ./build/GeneratedFiles/moc_QtSpeechMac.cpp ./build/GeneratedFiles/qrc_PixmapButton.cpp ./build/GeneratedFiles/qrc_Icons.cpp ./build/GeneratedFiles/qrc_Images.cpp
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: build/GeneratedFiles/moc_BaseProject.cpp build/GeneratedFiles/moc_PixmapButton.cpp build/GeneratedFiles/moc_HelpButton.cpp build/GeneratedFiles/moc_FilterLineEdit.cpp build/GeneratedFiles/moc_LabelWithProgress.cpp build/GeneratedFiles/moc_LineEditWithProgress.cpp build/GeneratedFiles/moc_ColumnSelectorButton.cpp build/GeneratedFiles/moc_GroupingProxyModel.cpp build/GeneratedFiles/moc_UngroupProxyModel.cpp build/GeneratedFiles/moc_ProgressBar.cpp build/GeneratedFiles/moc_BaseWindow.cpp build/GeneratedFiles/moc_TogMeg.cpp build/GeneratedFiles/moc_TogMegProject.cpp build/GeneratedFiles/moc_QAModel.cpp build/GeneratedFiles/moc_Examinator.cpp build/GeneratedFiles/moc_TaskController.cpp build/GeneratedFiles/moc_PlayController.cpp build/GeneratedFiles/moc_StudyController.cpp build/GeneratedFiles/moc_TogMegWindow.cpp build/GeneratedFiles/moc_FileNavigationView.cpp build/GeneratedFiles/moc_FileNavigationModel.cpp build/GeneratedFiles/moc_VHeader.cpp build/GeneratedFiles/moc_QAEditor.cpp build/GeneratedFiles/moc_QADelegate.cpp build/GeneratedFiles/moc_TimeLineWidget.cpp build/GeneratedFiles/moc_ExamineWidget.cpp build/GeneratedFiles/moc_QuestionWidget.cpp build/GeneratedFiles/moc_AnswerWidget.cpp build/GeneratedFiles/moc_RoundedLineEdit.cpp build/GeneratedFiles/moc_GradientWidget.cpp build/GeneratedFiles/moc_QtSpeech.cpp build/GeneratedFiles/moc_QtSpeechMac.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/GeneratedFiles/moc_BaseProject.cpp build/GeneratedFiles/moc_PixmapButton.cpp build/GeneratedFiles/moc_HelpButton.cpp build/GeneratedFiles/moc_FilterLineEdit.cpp build/GeneratedFiles/moc_LabelWithProgress.cpp build/GeneratedFiles/moc_LineEditWithProgress.cpp build/GeneratedFiles/moc_ColumnSelectorButton.cpp build/GeneratedFiles/moc_GroupingProxyModel.cpp build/GeneratedFiles/moc_UngroupProxyModel.cpp build/GeneratedFiles/moc_ProgressBar.cpp build/GeneratedFiles/moc_BaseWindow.cpp build/GeneratedFiles/moc_TogMeg.cpp build/GeneratedFiles/moc_TogMegProject.cpp build/GeneratedFiles/moc_QAModel.cpp build/GeneratedFiles/moc_Examinator.cpp build/GeneratedFiles/moc_TaskController.cpp build/GeneratedFiles/moc_PlayController.cpp build/GeneratedFiles/moc_StudyController.cpp build/GeneratedFiles/moc_TogMegWindow.cpp build/GeneratedFiles/moc_FileNavigationView.cpp build/GeneratedFiles/moc_FileNavigationModel.cpp build/GeneratedFiles/moc_VHeader.cpp build/GeneratedFiles/moc_QAEditor.cpp build/GeneratedFiles/moc_QADelegate.cpp build/GeneratedFiles/moc_TimeLineWidget.cpp build/GeneratedFiles/moc_ExamineWidget.cpp build/GeneratedFiles/moc_QuestionWidget.cpp build/GeneratedFiles/moc_AnswerWidget.cpp build/GeneratedFiles/moc_RoundedLineEdit.cpp build/GeneratedFiles/moc_GradientWidget.cpp build/GeneratedFiles/moc_QtSpeech.cpp build/GeneratedFiles/moc_QtSpeechMac.cpp
build/GeneratedFiles/moc_BaseProject.cpp: Code/Utils/BaseProject.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/BaseProject.h -o build/GeneratedFiles/moc_BaseProject.cpp

build/GeneratedFiles/moc_PixmapButton.cpp: Code/Utils/PixmapButton/PixmapButton.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/PixmapButton/PixmapButton.h -o build/GeneratedFiles/moc_PixmapButton.cpp

build/GeneratedFiles/moc_HelpButton.cpp: Code/Utils/HelpButton.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/HelpButton.h -o build/GeneratedFiles/moc_HelpButton.cpp

build/GeneratedFiles/moc_FilterLineEdit.cpp: Code/Utils/FilterLineEdit.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/FilterLineEdit.h -o build/GeneratedFiles/moc_FilterLineEdit.cpp

build/GeneratedFiles/moc_LabelWithProgress.cpp: Code/Utils/LabelWithProgress.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/LabelWithProgress.h -o build/GeneratedFiles/moc_LabelWithProgress.cpp

build/GeneratedFiles/moc_LineEditWithProgress.cpp: Code/Utils/LineEditWithProgress.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/LineEditWithProgress.h -o build/GeneratedFiles/moc_LineEditWithProgress.cpp

build/GeneratedFiles/moc_ColumnSelectorButton.cpp: Code/Utils/ColumnSelectorButton.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/ColumnSelectorButton.h -o build/GeneratedFiles/moc_ColumnSelectorButton.cpp

build/GeneratedFiles/moc_GroupingProxyModel.cpp: Code/Utils/GroupingProxyModel.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/GroupingProxyModel.h -o build/GeneratedFiles/moc_GroupingProxyModel.cpp

build/GeneratedFiles/moc_UngroupProxyModel.cpp: Code/Utils/UngroupProxyModel.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/UngroupProxyModel.h -o build/GeneratedFiles/moc_UngroupProxyModel.cpp

build/GeneratedFiles/moc_ProgressBar.cpp: Code/Utils/ProgressBar.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/ProgressBar.h -o build/GeneratedFiles/moc_ProgressBar.cpp

build/GeneratedFiles/moc_BaseWindow.cpp: Code/Utils/BaseWindow.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/Utils/BaseWindow.h -o build/GeneratedFiles/moc_BaseWindow.cpp

build/GeneratedFiles/moc_TogMeg.cpp: Code/TogMeg/TogMeg.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/TogMeg.h -o build/GeneratedFiles/moc_TogMeg.cpp

build/GeneratedFiles/moc_TogMegProject.cpp: Code/TogMeg/TogMegProject.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/TogMegProject.h -o build/GeneratedFiles/moc_TogMegProject.cpp

build/GeneratedFiles/moc_QAModel.cpp: Code/TogMeg/QAModel.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/QAModel.h -o build/GeneratedFiles/moc_QAModel.cpp

build/GeneratedFiles/moc_Examinator.cpp: Code/TogMeg/Examinator.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Examinator.h -o build/GeneratedFiles/moc_Examinator.cpp

build/GeneratedFiles/moc_TaskController.cpp: Code/TogMeg/TaskController.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/TaskController.h -o build/GeneratedFiles/moc_TaskController.cpp

build/GeneratedFiles/moc_PlayController.cpp: Code/TogMeg/TaskController.h \
		Code/TogMeg/PlayController.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/PlayController.h -o build/GeneratedFiles/moc_PlayController.cpp

build/GeneratedFiles/moc_StudyController.cpp: Code/TogMeg/TaskController.h \
		Code/TogMeg/StudyController.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/StudyController.h -o build/GeneratedFiles/moc_StudyController.cpp

build/GeneratedFiles/moc_TogMegWindow.cpp: Code/TogMeg/Gui/TogMegWindow.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Gui/TogMegWindow.h -o build/GeneratedFiles/moc_TogMegWindow.cpp

build/GeneratedFiles/moc_FileNavigationView.cpp: Code/TogMeg/Gui/FileNavigationView.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Gui/FileNavigationView.h -o build/GeneratedFiles/moc_FileNavigationView.cpp

build/GeneratedFiles/moc_FileNavigationModel.cpp: Code/TogMeg/Gui/FileNavigationModel.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Gui/FileNavigationModel.h -o build/GeneratedFiles/moc_FileNavigationModel.cpp

build/GeneratedFiles/moc_VHeader.cpp: Code/TogMeg/Editor/VHeader.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Editor/VHeader.h -o build/GeneratedFiles/moc_VHeader.cpp

build/GeneratedFiles/moc_QAEditor.cpp: Code/TogMeg/Editor/QAEditor.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Editor/QAEditor.h -o build/GeneratedFiles/moc_QAEditor.cpp

build/GeneratedFiles/moc_QADelegate.cpp: Code/TogMeg/Editor/QADelegate.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Editor/QADelegate.h -o build/GeneratedFiles/moc_QADelegate.cpp

build/GeneratedFiles/moc_TimeLineWidget.cpp: Code/TogMeg/Player/TimeLineWidget.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Player/TimeLineWidget.h -o build/GeneratedFiles/moc_TimeLineWidget.cpp

build/GeneratedFiles/moc_ExamineWidget.cpp: Code/TogMeg/Player/ExamineWidget.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Player/ExamineWidget.h -o build/GeneratedFiles/moc_ExamineWidget.cpp

build/GeneratedFiles/moc_QuestionWidget.cpp: Code/TogMeg/Player/GradientWidget.h \
		Code/TogMeg/Player/QuestionWidget.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Player/QuestionWidget.h -o build/GeneratedFiles/moc_QuestionWidget.cpp

build/GeneratedFiles/moc_AnswerWidget.cpp: Code/TogMeg/Player/GradientWidget.h \
		Code/TogMeg/Player/AnswerWidget.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Player/AnswerWidget.h -o build/GeneratedFiles/moc_AnswerWidget.cpp

build/GeneratedFiles/moc_RoundedLineEdit.cpp: Code/TogMeg/Player/RoundedLineEdit.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Player/RoundedLineEdit.h -o build/GeneratedFiles/moc_RoundedLineEdit.cpp

build/GeneratedFiles/moc_GradientWidget.cpp: Code/TogMeg/Player/GradientWidget.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/TogMeg/Player/GradientWidget.h -o build/GeneratedFiles/moc_GradientWidget.cpp

build/GeneratedFiles/moc_QtSpeech.cpp: Code/QtSpeech/QtSpeech.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/QtSpeech/QtSpeech.h -o build/GeneratedFiles/moc_QtSpeech.cpp

build/GeneratedFiles/moc_QtSpeechMac.cpp: Code/QtSpeech/QtSpeech.h \
		Code/QtSpeech/QtSpeechMac.h
	/usr/local/Trolltech/Qt-4.7.3/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Code/QtSpeech/QtSpeechMac.h -o build/GeneratedFiles/moc_QtSpeechMac.cpp

compiler_rcc_make_all: build/GeneratedFiles/qrc_PixmapButton.cpp build/GeneratedFiles/qrc_Icons.cpp build/GeneratedFiles/qrc_Images.cpp
compiler_rcc_clean:
	-$(DEL_FILE) build/GeneratedFiles/qrc_PixmapButton.cpp build/GeneratedFiles/qrc_Icons.cpp build/GeneratedFiles/qrc_Images.cpp
build/GeneratedFiles/qrc_PixmapButton.cpp: Code/Utils/PixmapButton/PixmapButton.qrc
	/usr/local/Trolltech/Qt-4.7.3/bin/rcc -name PixmapButton Code/Utils/PixmapButton/PixmapButton.qrc -o build/GeneratedFiles/qrc_PixmapButton.cpp

build/GeneratedFiles/qrc_Icons.cpp: Graphics/Icons.qrc
	/usr/local/Trolltech/Qt-4.7.3/bin/rcc -name Icons Graphics/Icons.qrc -o build/GeneratedFiles/qrc_Icons.cpp

build/GeneratedFiles/qrc_Images.cpp: Graphics/Images.qrc
	/usr/local/Trolltech/Qt-4.7.3/bin/rcc -name Images Graphics/Images.qrc -o build/GeneratedFiles/qrc_Images.cpp

compiler_image_collection_make_all: build/GeneratedFiles/qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) build/GeneratedFiles/qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean 


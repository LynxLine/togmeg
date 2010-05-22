//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include "BaseWindow.h"

class CrammeroProject;

/*!
 */
class MainWindow : public BaseWindow
{
Q_OBJECT

public:
    enum ViewMode {
        BrowserMode = 1,
        TaskEditorMode,
        ExamineMode
    };
	
    MainWindow(CrammeroProject * project, QWidget * parent = 0, Qt::WFlags flags = 0);
    virtual ~MainWindow();

    MainWindow::ViewMode viewMode();
    static QFont baseFont(qreal multiplier = 1.0, int weight = 20);

    CrammeroProject * project() const;
    
public slots:
    virtual void openFile();
    virtual void openFile(QString);

    void newEntry();
    void previousWindow();

    void runDemo();
    void runStudy();
    void stop();

    void quit();
    void openHelp();
    void openAbout();

    void switchFullScreen();
    void setViewMode(MainWindow::ViewMode);

signals:
    void viewModeChanged(MainWindow::ViewMode);

private slots:
	void createActions();
    void connectActions();
    void createShortcuts();
    void createMenuBar();
    void createToolBar();
    
    QString release() const;

private:
	class Private;
	Private * d;

};

#endif // MAINWINDOW_H

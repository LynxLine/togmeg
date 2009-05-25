//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#define _action(name) MainWindow::instance()->action(name)

/*!
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    enum ViewMode {
        BrowserMode = 1,
        TaskEditorMode,
        ExamineMode
    };
	
    MainWindow();
    virtual ~MainWindow();
    static MainWindow * instance();

    QAction * action(QString name);
    QActionGroup * actionGroup(QString name);

    MainWindow::ViewMode viewMode();
    static QFont baseFont(qreal multiplier = 1.0, int weight = 20);

public slots:
    void openFile();
    void saveFile();
    void saveFileAs();

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

private:
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

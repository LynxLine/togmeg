
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include "BaseWindow.h"

class TogMegProject;

/*!
 */
class TogMegWindow : public BaseWindow
{
Q_OBJECT

public:
    enum ViewMode {
        BrowserMode = 1,
        TaskEditorMode,
        ExamineMode
    };
	
    TogMegWindow(TogMegProject * project, QWidget * parent = 0, Qt::WFlags flags = 0);
    virtual ~TogMegWindow();

    TogMegWindow::ViewMode viewMode();
    static QFont baseFont(qreal multiplier = 1.0, int weight = 20);

    TogMegProject * project() const;
    
public slots:
    void newEntry();
    void previousWindow();

    void runDemo();
    void runStudy();
    void stop();

    void quit();
    void openHelp();
    void openAbout();

    void switchFullScreen();
    void setViewMode(TogMegWindow::ViewMode);

signals:
    void viewModeChanged(TogMegWindow::ViewMode);

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

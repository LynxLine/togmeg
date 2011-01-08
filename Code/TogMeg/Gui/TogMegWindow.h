
#ifndef TogMegWindow_H
#define TogMegWindow_H

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
    QDockWidget * filesDock() const;

public slots:
    void newFile();
    bool openFile(QString path);
    void newEntry();
    void swapQA();
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

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
	void createActions();
    void connectActions();
    void createMenuBar();
    void createToolBar();
    void setNextByRows();
    void setNextByCells();
    
    QString release() const;
    void adjustSpacerInToolBar(int);

private:
	class Private;
	Private * d;

};

#endif // TogMegWindow_H

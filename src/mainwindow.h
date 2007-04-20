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
        CatalogMode = 1,
        BrowserMode,
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
    void importFile();
    void exportFile();

    void newEntry();
    void openTaskEditor(QString taskId);
    void previousWindow();

    void runDemo();
    void runStudy();
    void runExamine();
    void stop();

    void quit();
    void openHelp();
    void openAbout();

    void switchFullScreen();

    void messageReceived(const QString &);
    void setViewMode(MainWindow::ViewMode);

signals:
    void viewModeChanged(MainWindow::ViewMode);

protected:
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void setCurrentTask(QString taskId);

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

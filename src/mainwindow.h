//
// Copyright (C) 2006 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef F4MAINWINDOW_H
#define F4MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#define _action(name) MainWindow::instance()->action(name)

/*!
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();
    virtual ~MainWindow();
    static MainWindow * instance();

    QAction * action(QString name);

    static QFont baseFont(qreal multiplier = 1.0, int weight = 20);

public slots:
    void quit();
    void openTask();
    void openHelp();
    void openAbout();
    void switchFullScreen();

    void messageReceived(const QString &);

private:
	void createActions();
    void connectActions();
    void createShortcuts();
    void createMenuBar();

    QString release() const;

private:
	class Private;
	Private * d;

};

#endif

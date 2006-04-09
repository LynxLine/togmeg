// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/*!
 The Main Window of aplication.
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget* parent = 0, Qt::WFlags f = Qt::Window );
    static MainWindow * self();
    ~MainWindow();

public slots:

private slots:

private:

protected:
    virtual void showEvent(QShowEvent *);
    virtual void mousePressEvent(QMouseEvent *);

private:
    class Private;
    Private * d;
};

#endif


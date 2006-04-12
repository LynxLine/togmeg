// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TimeoutPanel;
class AnswerWindow;
class QuestionWindow;
class TestEditorWindow;
class TestDescriptionWindow;

/*!
 The Main Window of aplication.
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    enum ViewMode { TestPropertiesMode = 1, StudyingMode };
    MainWindow(QWidget* parent = 0, Qt::WFlags f = Qt::Window );
    static MainWindow * self();
    ~MainWindow();

    TimeoutPanel * timeoutPanel();
    AnswerWindow * answerWindow();
    QuestionWindow * questionWindow();
    TestEditorWindow * testEditorWindow();
    TestDescriptionWindow * testDescriptionWindow();
    
    ViewMode viewMode();
    
public slots:
    void setViewMode(MainWindow::ViewMode);

private slots:

private:

protected:
    virtual void showEvent(QShowEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void mousePressEvent(QMouseEvent *);

private:
    class Private;
    Private * d;
};

#endif


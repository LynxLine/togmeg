// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include <QtSql>
#include <QtCore>

#include "mainwindow.h"
#include "timeoutpanel.h"
#include "answerwindow.h"
#include "questionwindow.h"
#include "studyprocessor.h"

#include "testprocessor.h"
#include "testeditorwindow.h"
#include "testdescriptionwindow.h"

class MainWindow::Private
{
public:
    Private(MainWindow * mw) {
        self=mw;
        viewMode = MainWindow::ViewMode(0); //undefined
    }
    static MainWindow * self;

    TimeoutPanel * timeoutPanel;
    AnswerWindow * answerWindow;
    QuestionWindow * questionWindow;
    TestEditorWindow * testEditorWindow;
    TestDescriptionWindow * testDescriptionWindow;

    TestProcessor * testProcessor;
    StudyProcessor * studyProcessor;

    MainWindow::ViewMode viewMode;
};

MainWindow * MainWindow::Private::self = 0L;

/*!
 Creates MainWindow object.
 */
MainWindow::MainWindow(QWidget* parent, Qt::WFlags f)
:QMainWindow(parent, f)
{
    d = new Private(this);

    setWindowTitle("aLesson");

    // qsettings stuff
    QCoreApplication::setOrganizationName("UAELTechnology");
    QCoreApplication::setOrganizationDomain("uael.com.ua");
    QCoreApplication::setApplicationName("aLesson");

    {
        QPalette pal = palette();
        pal.setBrush( backgroundRole(), QBrush(QColor("#888")) );
        setPalette( pal );
    }
    
	statusBar()->hide();
    menuBar()->hide();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath()+"/aLesson.db");
    bool ok = db.open();
    if (!ok) qDebug() << tr("Cannot open default database.");
    
    qApp->setActiveWindow(this);
    resize(800, 600);
    
    d->timeoutPanel = new TimeoutPanel(this);
    d->answerWindow = new AnswerWindow(this);
    d->questionWindow = new QuestionWindow(this);
    d->testEditorWindow = new TestEditorWindow(this);
    d->testDescriptionWindow = new TestDescriptionWindow(this);
    
    d->testProcessor = new TestProcessor(this);
    d->studyProcessor = new StudyProcessor(this);
    
    //showFullScreen();
    resize(800, 600);
}

/*!
 Deletes the object.
 */
MainWindow::~MainWindow()
{
    delete d;
}

/*!
 Returns the MainWindow object.
 Useful for accessing from other classes.
 */
MainWindow * MainWindow::self()
{
    return MainWindow::Private::self;
}

void MainWindow::showEvent(QShowEvent * se)
{
    QMainWindow::showEvent(se);
    
    //QTimer::singleShot(500, d->studyProcessor, SLOT(start()));
    setViewMode( TestPropertiesMode );
}

void MainWindow::keyPressEvent(QKeyEvent * ke)
{
    if (ke->key() == Qt::Key_Escape) {
        if (viewMode() == StudyingMode) d->studyProcessor->stop();
        else if (viewMode() == TestPropertiesMode) d->testProcessor->stop();
    
        QTimer::singleShot(500, qApp, SLOT(quit()));
    }
}

TimeoutPanel * MainWindow::timeoutPanel()
{
    return d->timeoutPanel;
}

AnswerWindow * MainWindow::answerWindow()
{
    return d->answerWindow;
}

QuestionWindow * MainWindow::questionWindow()
{
    return d->questionWindow;
}

TestEditorWindow * MainWindow::testEditorWindow()
{
    return d->testEditorWindow;
}

TestDescriptionWindow * MainWindow::testDescriptionWindow()
{
    return d->testDescriptionWindow;
}

MainWindow::ViewMode MainWindow::viewMode()
{
    return d->viewMode;
}

void MainWindow::setViewMode(MainWindow::ViewMode mode)
{
    if (mode == d->viewMode) return;

    if (d->viewMode == StudyingMode) d->studyProcessor->stop();
    if (d->viewMode == TestPropertiesMode) d->testProcessor->stop();
    
    if (mode == StudyingMode) d->studyProcessor->start();
    if (mode == TestPropertiesMode) d->testProcessor->start();

    d->viewMode = mode;
}


//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"
#include "catalogwidget.h"
#include "examinewidget.h"
#include "taskeditorwidget.h"

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

#define DEFAULT_SIZE QRect(30, 60, 700, 650)

void logMessageHandler(QtMsgType type, const char *msg);

class MainWindow::Private
{
public:
    Private(MainWindow * p)
    :viewMode(MainWindow::ViewMode(-1)) {
        instance = p;
    }

    static MainWindow * instance;

    QMap<QString, QAction *> actions;
    MainWindow::ViewMode viewMode;
    
    //gui
    QStackedWidget * stack;
    CatalogWidget * catalogWidget;
    ExamineWidget * examineWidget;
    TaskEditorWidget * taskEditorWidget;
};

MainWindow * MainWindow::Private::instance = 0L;

/*!
 Creates new MainWindow
 */
MainWindow::MainWindow()
:QMainWindow(0L, Qt::Window)
{
    d = new Private(this);

    QSettings s;

    //first geometry properies
    QByteArray ba = s.value("geometry/application").toByteArray();
    if (ba.isEmpty()) {
        setGeometry( DEFAULT_SIZE );
    }
    else restoreGeometry(ba);

    setWindowTitle("Crammero");
    
	createActions();
    createShortcuts();
    connectActions();
    createMenuBar();
    createToolBar();
	
	statusBar()->hide();

    d->stack = new QStackedWidget(this);
    d->stack->setFont( baseFont() );
    setCentralWidget( d->stack );

    d->catalogWidget = new CatalogWidget( d->stack );
    d->stack->addWidget( d->catalogWidget );

    d->examineWidget = new ExamineWidget( d->stack );
    d->stack->addWidget( d->examineWidget );

    d->taskEditorWidget = new TaskEditorWidget( d->stack );
    d->stack->addWidget( d->taskEditorWidget );

    setViewMode(MainWindow::CatalogMode);
}

/*!
 Deletes the object.
 */
MainWindow::~MainWindow()
{
    QSettings s;
    if (!isMaximized() && !isFullScreen()) s.setValue("geometry/application", saveGeometry());    
    s.setValue("geometry/maximized", isMaximized());
    s.setValue("geometry/fullscreen", isFullScreen());
    s.sync();
    
    d->actions.clear();

    delete d;
}

/*!
 Returns the MainWindow object.
 Useful for accessing from other classes.
 */
MainWindow * MainWindow::instance()
{
    return MainWindow::Private::instance;
}

/*!
 Returns QAction object by name.
 */
QAction * MainWindow::action(QString name)
{
    if (!d->actions.contains(name))
        qDebug() << "Not found action: " << name;

    Q_ASSERT(d->actions[name]);
    return d->actions[name];
}

/*!
 Returns current build number
 */
QString MainWindow::release() const
{
    QString date(__DATE__);
    QString m = date.left(3);
    QStringList mlist;
    mlist << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun"
	      << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";

    int month = mlist.indexOf(m) + 1;
    int day   = date.mid(4, 2).trimmed().toInt();
    int year  = date.mid(7, 4).trimmed().toInt();
    return QString::number(year*10000+month*100+day);
}

/*!
 Creates main actions for the application.
 */
void MainWindow::createActions()
{
	d->actions["app/exit"]   = new QAction (tr("E&xit"), this);
	d->actions["app/import"] = new QAction (tr("&Import..."), this);
    d->actions["app/export"] = new QAction (tr("&Export..."), this);
    d->actions["app/print"]  = new QAction (tr("&Print..."), this);

	d->actions["app/undo" ] = new QAction (tr("&Undo"), this);
	d->actions["app/redo" ] = new QAction (tr("&Redo"), this);
    d->actions["app/cut"  ] = new QAction (tr("Cu&t"), this);
    d->actions["app/copy" ] = new QAction (tr("&Copy"), this);
    d->actions["app/paste"] = new QAction (tr("&Paste"), this);

    d->actions["app/demo" ] = new QAction (QIcon(":/images/icons/play-32x32.png"), tr("&Play"), this);
    d->actions["app/study"] = new QAction (QIcon(":/images/icons/study-32x32.png"), tr("&Study"), this);
    d->actions["app/exam" ] = new QAction (QIcon(":/images/icons/examine-32x32.png"), tr("&Examinate"), this);
    d->actions["app/stop" ] = new QAction (QIcon(":/images/icons/stop-32x32.png"), tr("&Stop"), this);

	d->actions["category/add_category"    ] = new QAction (tr("&Create new Category"), this);
    d->actions["category/add_task"    ] = new QAction (tr("&Add new Task"), this);
    d->actions["category/remove" ] = new QAction (tr("&Remove"), this);

    d->actions["app/about"]         = new QAction (tr("&About"), this);
    d->actions["app/help"]          = new QAction (tr("Crammero &Help"), this);
    d->actions["app/check_updates"] = new QAction (tr("Check for Updates Now"), this);
}

/*!
 Creates menubar and add actions.
 */
void MainWindow::createMenuBar()
{
    QMenu * menu;
	menu = menuBar()->addMenu(tr("&File"));
	menu->addAction( action("app/import") );
	menu->addAction( action("app/export") );
	menu->addSeparator();
	menu->addAction( action("app/print") );
	menu->addSeparator();
	menu->addAction( action("app/exit") );

	menu = menuBar()->addMenu(tr("&Edit"));
	menu->addAction( action("app/undo") );
	menu->addAction( action("app/redo") );
	menu->addSeparator();
	menu->addAction( action("app/cut") );
	menu->addAction( action("app/copy") );
	menu->addAction( action("app/paste") );

	menu = menuBar()->addMenu(tr("&Run"));
	menu->addAction( action("app/demo") );
	menu->addAction( action("app/study") );
	menu->addAction( action("app/exam") );
	menu->addSeparator();
	menu->addAction( action("app/stop") );

    menu = menuBar()->addMenu(tr("&Help"));
	menu->addAction( action("app/about") );
	menu->addAction( action("app/help") );
    menu->addSeparator();
    menu->addAction( action("app/check_updates") );
}

void MainWindow::createToolBar()
{
    QToolBar * toolBar;
	toolBar = addToolBar(tr("Run"));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->setIconSize(QSize(32, 32));
    toolBar->setMovable(false);

	toolBar->addAction( action("app/import") );
	toolBar->addAction( action("app/export") );

    toolBar->addSeparator();
	toolBar->addAction( action("app/demo") );
	toolBar->addAction( action("app/study") );
	toolBar->addAction( action("app/exam") );
	toolBar->addSeparator();
	toolBar->addAction( action("app/stop") );
    
}

/*!
 Creates action shortcuts.
 */
void MainWindow::createShortcuts()
{
    // shortcuts
    action("app/print")      ->setShortcut(tr("Ctrl+P"));
    action("app/exit")       ->setShortcut(tr("Ctrl+Q"));
    action("app/undo")       ->setShortcut(tr("Ctrl+U"));
    action("app/redo")       ->setShortcut(tr("Ctrl+Y"));
    action("app/cut")        ->setShortcut(tr("Ctrl+X"));
    action("app/copy")       ->setShortcut(tr("Ctrl+C"));
    action("app/paste")      ->setShortcut(tr("Ctrl+V"));
    action("app/help")       ->setShortcut(tr("F1"));
}

/*!
 Connects actions to appropriate slots.
 */
void MainWindow::connectActions()
{
    connect( action("app/exit"),   SIGNAL(triggered()), this, SLOT(quit()));
    connect( action("app/help"),   SIGNAL(triggered()), this, SLOT(openHelp()));
    connect( action("app/about"),  SIGNAL(triggered()), this, SLOT(openAbout()));

    connect( action("app/import"), SIGNAL(triggered()), this, SLOT(importFile()));
    connect( action("app/export"), SIGNAL(triggered()), this, SLOT(exportFile()));

    connect( action("app/demo" ), SIGNAL(triggered()), this, SLOT(runDemo()));
    connect( action("app/study"), SIGNAL(triggered()), this, SLOT(runStudy()));
    connect( action("app/exam" ), SIGNAL(triggered()), this, SLOT(runExamine()));
    connect( action("app/stop" ), SIGNAL(triggered()), this, SLOT(stop()));

}

/*!
 Shows file open dialog.
 */
void MainWindow::importFile()
{
    /*
	QString filePath = QFileDialog::getOpenFileName(this,
        "Choose a file to open", ".", "Tasks (*.zip)" );

    if ( !filePath.isEmpty() ) {
        QFileInfo fi(filePath);
    }
    */
}

/*!
 Shows file save dialog.
 */
void MainWindow::exportFile()
{
    /*
	QString filePath = QFileDialog::getSaveFileName(this,
        "Choose a file to open", ".", "Tasks (*.zip)" );

    if ( !filePath.isEmpty() ) {
        QFileInfo fi(filePath);
    }
    */
}

void MainWindow::editStudy()
{
    setViewMode(MainWindow::TaskEditorMode);
}

void MainWindow::runDemo()
{
    setViewMode(MainWindow::ExamineMode);
}

void MainWindow::runStudy()
{
    setViewMode(MainWindow::ExamineMode);
}

void MainWindow::runExamine()
{
    setViewMode(MainWindow::ExamineMode);
}

void MainWindow::stop()
{
    setViewMode(MainWindow::CatalogMode);
}

/*!
 * Opens default browser directed to help page
 */
void MainWindow::openHelp()
{
    QString url = QString("http://www.lynxline.com");
    QDesktopServices::openUrl(url);
}

void MainWindow::openAbout()
{
}

void MainWindow::switchFullScreen()
{
    if (isFullScreen()) showNormal();
    else                showFullScreen();
}

void MainWindow::quit()
{
    close();
    qApp->closeAllWindows();
}

void MainWindow::messageReceived(const QString & message)
{
    qDebug() << "MainWindow::messageReceived()" << message;

    if (message == "Restore") {
        show();
        return;
    }

    if (message == "Stop") {
        quit();
        return;
    }
}

MainWindow::ViewMode MainWindow::viewMode()
{
    return d->viewMode;
}

void MainWindow::setViewMode(MainWindow::ViewMode m)
{
    d->viewMode = m;
    if (m == MainWindow::CatalogMode) {
        d->stack->setCurrentWidget( d->catalogWidget );
    }
    else if (m == MainWindow::TaskEditorMode) {
        d->stack->setCurrentWidget( d->taskEditorWidget );
    }
    else if (m == MainWindow::ExamineMode) {
        d->stack->setCurrentWidget( d->examineWidget );
    }

    action("app/demo" )->setEnabled( m!=MainWindow::ExamineMode );
    action("app/study")->setEnabled( m!=MainWindow::ExamineMode );
    action("app/exam" )->setEnabled( m!=MainWindow::ExamineMode );
    action("app/stop" )->setEnabled( m==MainWindow::ExamineMode );

    emit viewModeChanged(m);
}

QFont MainWindow::baseFont(qreal multiplier, int weight)
{
    static qreal basePointSize = 1.0;
    static bool initialized = false;
    if (!initialized) {
        // we need to get 14 pixel height of our font
        qreal height = 1.0;
        do {
            basePointSize += 0.05;
            QFont f("Verdana", 10);
            f.setPointSizeF(basePointSize);
            QFontMetricsF fm(f);
            height = fm.height();

            if (basePointSize > 100.0) {
                basePointSize = 9.0;
                initialized = true;
                break;
            }
        }
        while (height<15.0);
        initialized = true;
    }


    QFont font( "Verdana", 10, weight );
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setPointSizeF(basePointSize * multiplier);
    return font;
}

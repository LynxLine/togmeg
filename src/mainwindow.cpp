//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"
#include "catalogwidget.h"
#include "examinewidget.h"

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

#define DEFAULT_SIZE QRect(30, 60, 800, 550)

void logMessageHandler(QtMsgType type, const char *msg);

class MainWindow::Private
{
public:
    Private(MainWindow * p)
    :viewMode(MainWindow::ViewMode(-1))
    {
        instance = p;
    }

    static MainWindow * instance;

    QMap<QString, QAction *> actions;
    MainWindow::ViewMode viewMode;
    
    //gui
    QStackedWidget * stack;
    CatalogWidget * catalogWidget;
    ExamineWidget * examineWidget;
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
    //if (ba.isEmpty()) {
#ifdef Q_WS_WIN
        {
            ::RECT drect;
            ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &drect, 0);
            int x, y, w, h;
            x = drect.left+5;
            y = drect.top+5;
            w = drect.right-drect.left-10;
            h = drect.bottom-drect.top-10;

            move(x,y);
            resize(w-8,h-35);
        }
#else
        setGeometry( DEFAULT_SIZE );
#endif
    //}
    //else restoreGeometry(ba);

    setWindowTitle("Serrater");
    
	createActions();
    createShortcuts();
    connectActions();
    createMenuBar();
	
	statusBar()->hide();

    d->stack = new QStackedWidget(this);
    setCentralWidget( d->stack );

    d->catalogWidget = new CatalogWidget( d->stack );
    d->stack->addWidget( d->catalogWidget );

    d->examineWidget = new ExamineWidget( d->stack );
    d->stack->addWidget( d->examineWidget );

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
	d->actions["ad/exit"]   = new QAction (tr("E&xit"), this);
	d->actions["ad/import"] = new QAction (tr("&Import..."), this);
    d->actions["ad/export"] = new QAction (tr("&Export..."), this);
    d->actions["ad/print"]  = new QAction (tr("&Print..."), this);

	d->actions["ad/undo" ] = new QAction (tr("&Undo"), this);
	d->actions["ad/redo" ] = new QAction (tr("&Redo"), this);
    d->actions["ad/cut"  ] = new QAction (tr("Cu&t"), this);
    d->actions["ad/copy" ] = new QAction (tr("&Copy"), this);
    d->actions["ad/paste"] = new QAction (tr("&Paste"), this);

	d->actions["ad/demo" ] = new QAction (tr("&Demo"), this);
    d->actions["ad/study"] = new QAction (tr("&Study"), this);
    d->actions["ad/exam" ] = new QAction (tr("&Examinate"), this);
    d->actions["ad/stop" ] = new QAction (tr("&Stop"), this);

    d->actions["ad/about"]         = new QAction (tr("&About"), this);
    d->actions["ad/help"]          = new QAction (tr("Serrater &Help"), this);
    d->actions["ad/check_updates"] = new QAction (tr("Check for Updates Now"), this);
}

/*!
 Creates menubar and add actions.
 */
void MainWindow::createMenuBar()
{
    QMenu * menu;
	menu = menuBar()->addMenu(tr("&File"));
	menu->addAction( action("ad/import") );
	menu->addAction( action("ad/export") );
	menu->addSeparator();
	menu->addAction( action("ad/print") );
	menu->addSeparator();
	menu->addAction( action("ad/exit") );

	menu = menuBar()->addMenu(tr("&Edit"));
	menu->addAction( action("ad/undo") );
	menu->addAction( action("ad/redo") );
	menu->addSeparator();
	menu->addAction( action("ad/cut") );
	menu->addAction( action("ad/copy") );
	menu->addAction( action("ad/paste") );

	menu = menuBar()->addMenu(tr("&Run"));
	menu->addAction( action("ad/demo") );
	menu->addAction( action("ad/study") );
	menu->addAction( action("ad/exam") );
	menu->addSeparator();
	menu->addAction( action("ad/stop") );

    menu = menuBar()->addMenu(tr("&Help"));
	menu->addAction( action("ad/about") );
	menu->addAction( action("ad/help") );
    menu->addSeparator();
    menu->addAction( action("ad/check_updates") );
}

/*!
 Creates action shortcuts.
 */
void MainWindow::createShortcuts()
{
    // shortcuts
    action("ad/print")      ->setShortcut(tr("Ctrl+P"));
    action("ad/exit")       ->setShortcut(tr("Ctrl+Q"));
    action("ad/undo")       ->setShortcut(tr("Ctrl+U"));
    action("ad/redo")       ->setShortcut(tr("Ctrl+Y"));
    action("ad/cut")        ->setShortcut(tr("Ctrl+X"));
    action("ad/copy")       ->setShortcut(tr("Ctrl+C"));
    action("ad/paste")      ->setShortcut(tr("Ctrl+V"));
    action("ad/help")       ->setShortcut(tr("F1"));
}

/*!
 Connects actions to appropriate slots.
 */
void MainWindow::connectActions()
{
    connect( action("ad/exit"),   SIGNAL(triggered()), this, SLOT(quit()));
    connect( action("ad/help"),   SIGNAL(triggered()), this, SLOT(openHelp()));
    connect( action("ad/about"),  SIGNAL(triggered()), this, SLOT(openAbout()));

    connect( action("ad/import"), SIGNAL(triggered()), this, SLOT(importFile()));
    connect( action("ad/export"), SIGNAL(triggered()), this, SLOT(exportFile()));

   connect( action("ad/demo" ), SIGNAL(triggered()), this, SLOT(runDemo()));
   connect( action("ad/study"), SIGNAL(triggered()), this, SLOT(runStudy()));
   connect( action("ad/exam" ), SIGNAL(triggered()), this, SLOT(runExamine()));
   connect( action("ad/stop" ), SIGNAL(triggered()), this, SLOT(stop()));

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
    QString url = QString("http://www.serrater.com");
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
    else if (m == MainWindow::ExamineMode) {
        d->stack->setCurrentWidget( d->examineWidget );
    }

    action("ad/demo" )->setEnabled( m==MainWindow::CatalogMode );
    action("ad/study")->setEnabled( m==MainWindow::CatalogMode );
    action("ad/exam" )->setEnabled( m==MainWindow::CatalogMode );
    action("ad/stop" )->setEnabled( m==MainWindow::ExamineMode );

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

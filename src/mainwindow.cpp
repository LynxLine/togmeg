//
// Copyright (C) 2006 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

#define DEFAULT_SIZE QRect(30, 60, 800, 550)

void logMessageHandler(QtMsgType type, const char *msg);

class MainWindow::Private
{
public:
    Private(MainWindow * p){
        instance = p;
    }

    static MainWindow * instance;

    QMap<QString, QAction *> actions;

};

MainWindow * MainWindow::Private::instance = 0L;

/*!
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
 Creates main actions for the application.
 */
void MainWindow::createActions()
{
	d->actions["ad/exit"]           = new QAction (tr("E&xit"), this);
	d->actions["ad/open"]           = new QAction (tr("&Open a task..."), this);
    d->actions["ad/close"]          = new QAction (tr("&Close the task"), this);
    d->actions["ad/close_all"]      = new QAction (tr("Close &all tasks"), this);

    d->actions["ad/help"]                 = new QAction (tr("eLinker &Help"), this);
    d->actions["ad/check_updates"]        = new QAction (tr("Check for Updates Now"), this);
    d->actions["ad/about"]                = new QAction (tr("&About"), this);
}

/*!
 Creates menubar and add actions.
 */
void MainWindow::createMenuBar()
{
	QMenu * menu = menuBar()->addMenu(tr("&File"));
	menu->addAction( action("ad/open") );
	menu->addAction( action("ad/close") );
	menu->addAction( action("ad/close_all") );
	menu->addSeparator();
	menu->addAction( action("ad/exit") );

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
	action("ad/open")       ->setShortcut(tr("Ctrl+O"));
	action("ad/close")      ->setShortcut(tr("Ctrl+W"));
    action("ad/exit")       ->setShortcut(tr("Ctrl+Q"));
    action("ad/help")       ->setShortcut(tr("F1"));
}

/*!
 Connects actions to appropriate slots.
 */
void MainWindow::connectActions()
{
    connect( action("ad/exit"), SIGNAL(triggered()), this, SLOT(quit()));
    connect( action("ad/open"), SIGNAL(triggered()), this, SLOT(openTask()));
    connect( action("ad/close"), SIGNAL(triggered()), this, SLOT(closeTask()));
    connect( action("ad/close_all"), SIGNAL(triggered()), this, SLOT(closeAllTasks()));
    connect( action("ad/about"), SIGNAL(triggered()), this, SLOT(openAbout()));
    connect( action("ad/help"), SIGNAL(triggered()), this, SLOT(openHelp()));
}

/*!
 Shows file open dialog.
 */
void MainWindow::openTask()
{
	QString filePath = QFileDialog::getOpenFileName(this,
        "Choose a file to open", ".", "Tasks (*.zip)" );

    if ( !filePath.isEmpty() ) {
        QFileInfo fi(filePath);
    }
}

/*!
 * Opens default browser directed to help page
 */
void MainWindow::openHelp()
{
    QString url = QString("http://www.serrater.com");
    QDesktopServices::openUrl(url);
}


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

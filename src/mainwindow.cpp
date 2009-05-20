//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "crammero.h"
#include "mainwindow.h"
#include "examinator.h"

#include "examinewidget.h"
#include "taskeditorwidget.h"

#include "TaskEditorView.h"
#include "StudyTaskModel.h"

#include "AppStyles.h"

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

    QMap<QString, QActionGroup *> actionGroups;
    QMap<QString, QAction *> actions;
    MainWindow::ViewMode viewMode;
    
    //gui
    QStackedWidget * stack;
    
    ExamineWidget * examineWidget;
    TaskEditorWidget * taskEditorWidget;

    Examinator * examinator;

    QSizeGrip * sizeGrip;
};

MainWindow * MainWindow::Private::instance = 0L;

/*!
 Creates new MainWindow
 */
MainWindow::MainWindow()
:QMainWindow(0L, Qt::Window)
{
    d = new Private(this);

    //setup palette
    {
        QPalette palette = this->palette();
        palette.setColor(QPalette::Base, "#FFFFFF");
        palette.setColor(QPalette::AlternateBase, "#EDF3FE");
        palette.setColor(QPalette::Active,   QPalette::Highlight, "#3875D7");
        palette.setColor(QPalette::Inactive, QPalette::Highlight, "#A4B4CB");
        palette.setColor(QPalette::Active,   QPalette::HighlightedText, "#FFFFFF");
        palette.setColor(QPalette::Inactive, QPalette::HighlightedText, "#FFFFFF");
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, "#FFFFFF");
        setPalette( palette );
    }

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

#ifdef Q_WS_WIN
    d->stack->setFont( baseFont() );
#endif

#ifdef Q_WS_MAC
    d->stack->setFont( baseFont(0.97) );
#endif

    setCentralWidget( d->stack );

    d->taskEditorWidget = new TaskEditorWidget( d->stack );
    d->stack->addWidget( d->taskEditorWidget );
    
    d->examineWidget = new ExamineWidget( d->stack );
    d->examinator = d->examineWidget->examinator();
    d->stack->addWidget( d->examineWidget );

    setViewMode(MainWindow::TaskEditorMode);

    connect(d->examinator, SIGNAL(examinatorEnabled(bool)),
            actionGroup("examine"), SLOT(setEnabled(bool)));
    connect(d->examinator, SIGNAL(stopped()),
            this, SLOT(stop()));

    d->sizeGrip = new QSizeGrip(this);
    d->sizeGrip->raise();
#ifdef Q_WS_MAC
    QPalette palette = d->sizeGrip->palette();
    palette.setColor(QPalette::Window, QColor(0,0,0,0));
    d->sizeGrip->setPalette( palette );
#endif
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

    delete d->examineWidget;
    delete d->taskEditorWidget;

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
 Returns QActionGroup object by name.
 */
QActionGroup * MainWindow::actionGroup(QString name)
{
    if (!d->actionGroups.contains(name))
        qDebug() << "Not found action: " << name;

    Q_ASSERT(d->actionGroups[name]);
    return d->actionGroups[name];
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
	d->actions["app/Open"]   = new QAction (tr("Open"), this);
	d->actions["app/Save"]   = new QAction (tr("Save"), this);
	d->actions["app/SaveAs"] = new QAction (tr("Save As"), this);

    
	d->actions["app/exit"]   = new QAction (tr("E&xit"), this);
	d->actions["app/import"] = new QAction (tr("&Import..."), this);
    d->actions["app/export"] = new QAction (tr("&Export..."), this);
    d->actions["app/print"]  = new QAction (tr("&Print..."), this);

	d->actions["app/forward" ]  = new QAction (tr("&Forward"), this);

	d->actions["app/new"] = new QAction (QIcon(":/images/icons/Add.png"), tr("&Add Row"), this);

	d->actions["app/undo" ] = new QAction (tr("&Undo"), this);
	d->actions["app/redo" ] = new QAction (tr("&Redo"), this);
    d->actions["app/cut"  ] = new QAction (tr("Cu&t"), this);
    d->actions["app/copy" ] = new QAction (tr("&Copy"), this);
    d->actions["app/paste"] = new QAction (tr("&Paste"), this);

    d->actions["app/demo" ] = new QAction (QIcon(":/images/icons/Play.png"   ), tr("&Play"), this);
    d->actions["app/study"] = new QAction (QIcon(":/images/icons/Record.png" ), tr("&Study"), this);
    d->actions["app/stop" ] = new QAction (QIcon(":/images/icons/Stop.png"   ), tr("&Stop"), this);

    d->actions["app/about"]         = new QAction (tr("&About"), this);
    d->actions["app/help"]          = new QAction (tr("Crammero &Help"), this);
    d->actions["app/check_updates"] = new QAction (tr("Check for Updates Now"), this);

    d->actionGroups["examine"] = new QActionGroup(this);
    d->actionGroups["examine"]->setExclusive(false);
    d->actionGroups["examine"]->addAction( d->actions["app/demo" ] );
    d->actionGroups["examine"]->addAction( d->actions["app/study" ] );
}

/*!
 Creates menubar and add actions.
 */
void MainWindow::createMenuBar()
{
    QMenu * menu;
	menu = menuBar()->addMenu(tr("&File"));
	menu->addAction( action("app/Open") );
	menu->addAction( action("app/Save") );
	menu->addAction( action("app/SaveAs") );
	menu->addSeparator();
	menu->addAction( action("app/exit") );

	menu = menuBar()->addMenu(tr("&Edit"));
    menu->addAction( action("app/new") );

	menu = menuBar()->addMenu(tr("&Run"));
	menu->addAction( action("app/demo") );
	menu->addAction( action("app/study") );
	menu->addSeparator();
	menu->addAction( action("app/stop") );

    menu = menuBar()->addMenu(tr("&Help"));
	menu->addAction( action("app/about") );
    menu->addSeparator();
    menu->addAction( action("app/check_updates") );
}

void MainWindow::createToolBar()
{
	QToolBar * toolBar = addToolBar(tr("Toolbar"));

    QWidget * space1 = new QWidget;
    space1->setFixedSize(5,10);
    toolBar->addWidget(space1);

	toolBar->addAction( action("app/new") );

    QWidget * space3 = new QWidget;
    space3->setFixedSize(20,10);
    toolBar->addWidget(space3);

	toolBar->addAction( action("app/demo") );
	toolBar->addAction( action("app/study") );

    QWidget * space4 = new QWidget;
    space4->setFixedSize(10,10);
    toolBar->addWidget(space4);

    toolBar->addAction( action("app/stop") );
    
    toolBar->setIconSize(QSize(24, 24));
    toolBar->setMovable(false);
    
#ifdef Q_WS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
    toolBar->setStyleSheet(AppStyles::mac_toolbarStyle);
#endif
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
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
    connect( action("app/Open"),   SIGNAL(triggered()), this, SLOT(openFile()));
    connect( action("app/Save"),   SIGNAL(triggered()), this, SLOT(saveFile()));
    connect( action("app/SaveAs"), SIGNAL(triggered()), this, SLOT(saveFileAs()));

    connect( action("app/exit"),   SIGNAL(triggered()), this, SLOT(quit()));
    connect( action("app/help"),   SIGNAL(triggered()), this, SLOT(openHelp()));
    connect( action("app/about"),  SIGNAL(triggered()), this, SLOT(openAbout()));

    connect( action("app/new"),    SIGNAL(triggered()), this, SLOT(newEntry()));

    connect( action("app/import"), SIGNAL(triggered()), this, SLOT(importFile()));
    connect( action("app/export"), SIGNAL(triggered()), this, SLOT(exportFile()));

    connect( action("app/demo" ), SIGNAL(triggered()), this, SLOT(runDemo()));
    connect( action("app/study"), SIGNAL(triggered()), this, SLOT(runStudy()));
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

void MainWindow::newEntry()
{
    if ( viewMode() == MainWindow::TaskEditorMode) {
        d->taskEditorWidget->addNewEntry();
    }
}

void MainWindow::previousWindow()
{
    if ( viewMode() == ExamineMode ) setViewMode(MainWindow::TaskEditorMode);
}

void MainWindow::runDemo()
{
    d->examinator->start( Examinator::Playing );
    setViewMode(MainWindow::ExamineMode);
}

void MainWindow::runStudy()
{
    d->examinator->start( Examinator::Studying );
    setViewMode(MainWindow::ExamineMode);
}

void MainWindow::stop()
{
    if ( d->examinator->state() != Examinator::Stopped ) {
        setViewMode(MainWindow::BrowserMode);
        d->examinator->stop();
    }
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

MainWindow::ViewMode MainWindow::viewMode()
{
    return d->viewMode;
}

void MainWindow::setViewMode(MainWindow::ViewMode m)
{
    d->viewMode = m;
    if (m == MainWindow::TaskEditorMode) {
        //first switch stack
        if ( d->stack->currentWidget() != d->taskEditorWidget ) {
            d->stack->setCurrentWidget( d->taskEditorWidget );
        }
    }
    else if (m == MainWindow::ExamineMode) {
        //just switch stack
        d->stack->setCurrentWidget( d->examineWidget );
    }
    else if (m == MainWindow::BrowserMode) {
        //just switch stack
        if ( d->stack->currentWidget() != d->taskEditorWidget ) {
            d->stack->setCurrentWidget( d->taskEditorWidget );
        }

        d->viewMode = MainWindow::TaskEditorMode;
    }

    actionGroup("examine")->setEnabled( d->examinator->entryCount() >0 );
    action("app/demo" )->setEnabled( d->viewMode!=MainWindow::ExamineMode );
    action("app/study")->setEnabled( d->viewMode!=MainWindow::ExamineMode );
    action("app/stop" )->setEnabled( d->viewMode==MainWindow::ExamineMode );

    emit viewModeChanged( d->viewMode );
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

QFont MainWindow::systemFont()
{
    return instance()->font();
}

void MainWindow::resizeEvent(QResizeEvent * re)
{
    QSize size = re->size();
    d->sizeGrip->move( size.width()-d->sizeGrip->width(), size.height()-d->sizeGrip->height() );
    QMainWindow::resizeEvent(re);
}

void MainWindow::openFile()
{
    QString path;
    
    path = QFileDialog::getOpenFileName(
                                        this,
                                        tr("Open a crammero file"), path,
                                        tr("Xml files (*.xml);;Any file (*)")
                                        );
    
    if (path.isEmpty()) return;
    
    d->taskEditorWidget->view()->studyTaskModel()->loadFile(path);
    
    bool ok = true;//project()->loadFile(path);
    if (!ok) {
        QMessageBox box(QMessageBox::Information, tr("Load is failed"),
                        tr("Load of file \"%1\" is failed").arg(QFileInfo(path).fileName()),
                        QMessageBox::Ok, this);
        box.setInformativeText(tr("Possible format of the file is wrong."));
        box.setWindowModality(Qt::WindowModal);
        box.setDefaultButton(QMessageBox::Ok);
        
#ifdef Q_WS_WIN
        QString text = box.text();
        box.setText( tr("<font size='+1'>%1</font>").arg(text) );
#endif
        box.exec();
    }
    
}

void MainWindow::saveFile()
{
    saveFileAs();
}

void MainWindow::saveFileAs()
{
    QString path;
    
    path = QFileDialog::getSaveFileName(
                                        this,
                                        tr("Save a crammero file"), path,
                                        tr("Xml files (*.xml);;Any file (*)")
                                        );
    
    if (path.isEmpty()) return;
    
    d->taskEditorWidget->view()->studyTaskModel()->saveFile(path);
    
}

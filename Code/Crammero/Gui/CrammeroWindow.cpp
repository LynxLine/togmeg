//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "crammero.h"
#include "CrammeroWindow.h"
#include "examinator.h"

#include "BaseProject.h"

#include "examinewidget.h"
#include "CramFileWidget.h"

#include "CramFileView.h"
#include "CramFileModel.h"

#include "AppStyles.h"

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

class MainWindow::Private
{
public:
    MainWindow * instance;
    MainWindow::ViewMode viewMode;
    
    QPointer<StudyTaskModel> model;
    QPointer<Examinator> examinator;
    
    //gui
    QStackedWidget * stack;
    ExamineWidget * examineWidget;
    TaskEditorWidget * taskEditorWidget;
};

/*!
 Creates new MainWindow
 */
MainWindow::MainWindow(BaseProject * proj, QWidget * parent, Qt::WFlags flags)
:BaseWindow(proj, parent, flags)
{
    d = new Private;
    d->instance = this;
    d->viewMode = MainWindow::ViewMode(-1);

    d->model = new StudyTaskModel(this);
    d->examinator = new Examinator(d->model);
    
    QSettings s;

    setWindowTitle("Crammero");
    
	createActions();
    createShortcuts();
    connectActions();
    createMenuBar();
    createToolBar();
	
	statusBar()->hide();

    d->stack = new QStackedWidget(this);
    setCentralWidget( d->stack );

    d->taskEditorWidget = new TaskEditorWidget(d->model, d->stack);
    d->stack->addWidget( d->taskEditorWidget );
    
    d->examineWidget = new ExamineWidget(d->examinator, d->stack );
    d->stack->addWidget( d->examineWidget );

    setViewMode(MainWindow::TaskEditorMode);

    connect(d->examinator, SIGNAL(examinatorEnabled(bool)),
            actionGroup("Play"), SLOT(setEnabled(bool)));
    connect(d->examinator, SIGNAL(stopped()),
            this, SLOT(stop()));
    connect(d->model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            project(), SLOT(setModified()));
    
    readSettings();
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

    delete d->examineWidget;
    delete d->taskEditorWidget;

    delete d;
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
	setAction("Open"   , new QAction (tr("Open..."), this));
	setAction("Close"  , new QAction (tr("Close"), this));
	setAction("Save"   , new QAction (tr("Save"), this));
	setAction("SaveAs" , new QAction (tr("Save As..."), this));
	setAction("Quit"   , new QAction (tr("E&xit"), this));

	setAction("Add" , new QAction (QIcon(":/images/icons/Add.png"), tr("&Add Row"), this));

    setAction("Play"  , new QAction (QIcon(":/images/icons/Play.png"   ), tr("&Play"), this));
    setAction("Study" , new QAction (QIcon(":/images/icons/Record.png" ), tr("&Study"), this));
    setAction("Stop"  , new QAction (QIcon(":/images/icons/Stop.png"   ), tr("&Stop"), this));

    setAction("About"         , new QAction (tr("&About"), this));
    setAction("Help"          , new QAction (tr("Crammero &Help"), this));
    setAction("CheckUpdates" , new QAction (tr("Check for Updates Now"), this));

    setActionGroup("Play", new QActionGroup(this));
    actionGroup("Play")->setExclusive(false);
    actionGroup("Play")->addAction(action("Play"));
    actionGroup("Play")->addAction(action("Study"));
}

/*!
 Creates menubar and add actions.
 */
void MainWindow::createMenuBar()
{
    QMenu * menu;
	menu = menuBar()->addMenu(tr("&File"));
	menu->addAction( action("Open") );
	menu->addSeparator();
	menu->addAction( action("Close") );
	menu->addAction( action("Save") );
	menu->addAction( action("SaveAs") );
	menu->addSeparator();
	menu->addAction( action("Quit") );

	menu = menuBar()->addMenu(tr("&Edit"));
    menu->addAction( action("Add") );

	menu = menuBar()->addMenu(tr("&Run"));
	menu->addAction( action("Play") );
	menu->addAction( action("Study") );
	menu->addSeparator();
	menu->addAction( action("Stop") );

    menu = menuBar()->addMenu(tr("&Help"));
	menu->addAction( action("About") );
    menu->addSeparator();
    menu->addAction( action("CheckUpdates") );
}

void MainWindow::createToolBar()
{
	QToolBar * toolBar = addToolBar(tr("Toolbar"));
	toolBar->addAction( action("Add") );
	toolBar->addAction( action("Play") );
	toolBar->addAction( action("Study") );
    toolBar->addAction( action("Stop") );
    
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
    action("Open")   ->setShortcut(tr("Ctrl+O"));
    action("Close")  ->setShortcut(tr("Ctrl+W"));
    action("Save")   ->setShortcut(tr("Ctrl+S"));
    action("SaveAs") ->setShortcut(tr("Shift+Ctrl+S"));
    action("Quit")   ->setShortcut(tr("Ctrl+Q"));
    action("Help")   ->setShortcut(tr("F1"));
}

/*!
 Connects actions to appropriate slots.
 */
void MainWindow::connectActions()
{
    connect( action("Open"),   SIGNAL(triggered()), this, SLOT(openFile()));
    connect( action("Close"),  SIGNAL(triggered()), this, SLOT(close()));
    connect( action("Save"),   SIGNAL(triggered()), this, SLOT(saveFile()));
    connect( action("SaveAs"), SIGNAL(triggered()), this, SLOT(saveFileAs()));

    connect( action("Quit"),   SIGNAL(triggered()), this, SLOT(close()));
    connect( action("Help"),   SIGNAL(triggered()), this, SLOT(openHelp()));
    connect( action("About"),  SIGNAL(triggered()), this, SLOT(openAbout()));

    connect( action("Add"),    SIGNAL(triggered()), this, SLOT(newEntry()));

    connect( action("Play" ),  SIGNAL(triggered()), this, SLOT(runDemo()));
    connect( action("Study"),  SIGNAL(triggered()), this, SLOT(runStudy()));
    connect( action("Stop" ),  SIGNAL(triggered()), this, SLOT(stop()));
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

    //actionGroup("Play")->setEnabled( d->examinator->entryCount() >0 );
    action("Play" )->setEnabled( d->viewMode!=MainWindow::ExamineMode );
    action("Study")->setEnabled( d->viewMode!=MainWindow::ExamineMode );
    action("Stop" )->setEnabled( d->viewMode==MainWindow::ExamineMode );

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

void MainWindow::openFile()
{
    QString path;
    
    path = QFileDialog::getOpenFileName(this,
                                        tr("Open a crammero file"), path,
                                        tr("Tab delimited files (*.tab);;Xml files (*.xml);;Any file (*)")
                                        );
    
    if (path.isEmpty()) return;
    
    openFile(path);
}

void MainWindow::openFile(QString path)
{
    qDebug() << path;
    if (path.endsWith(".xml", Qt::CaseInsensitive))
        d->model->loadXmlFile(path);
    else d->model->loadTabFile(path);
    project()->setModified(false);
    
    bool ok = true;
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

bool MainWindow::saveFile()
{
    return saveFileAs();
}

bool MainWindow::saveFileAs()
{
    QString path = "words.tab";
    
    path = QFileDialog::getSaveFileName(this,
                                        tr("Save a crammero file"), path,
                                        tr("Tab delimited files (*.tab);;Xml files (*.xml);;Any file (*)")
                                        );
    
    if (path.isEmpty()) return false;
    
    bool ok = false;
    if (path.endsWith(".xml", Qt::CaseInsensitive))
        ok = d->model->saveXmlFile(path);
    else ok = d->model->saveTabFile(path);
    
    project()->setModified(false);
    return ok;
}

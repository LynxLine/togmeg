//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#include <QtCore>

#include "TogMegWindow.h"
#include "TogMegProject.h"

#include "crammero.h"
#include "examinator.h"

#include "examinewidget.h"
#include "CramFileWidget.h"

#include "CramFileView.h"
#include "CramFileModel.h"

#include "FileNavigationView.h"
#include "FileNavigationModel.h"

#include "AppStyles.h"

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

class TogMegWindow::Private
{
public:
    TogMegWindow * instance;
    TogMegWindow::ViewMode viewMode;
    
    QPointer<Examinator> examinator;
    
    QStackedWidget * stack;
    ExamineWidget * examineWidget;
    TaskEditorWidget * taskEditorWidget;
};

/*!
 Creates new TogMegWindow
 */
TogMegWindow::TogMegWindow(TogMegProject * proj, QWidget * parent, Qt::WFlags flags)
:BaseWindow(proj, parent, flags)
{
    d = new Private;
    d->instance = this;
    d->viewMode = TogMegWindow::ViewMode(-1);

    d->examinator = new Examinator(project()->model());
    
    QSettings s;

    setWindowTitle("TogMeg");
    
	createActions();
    createShortcuts();
    connectActions();
    createMenuBar();
    createToolBar();
	
	statusBar()->hide();

    d->stack = new QStackedWidget(this);
    setCentralWidget( d->stack );

    d->taskEditorWidget = new TaskEditorWidget(project()->model(), d->stack);
    d->stack->addWidget( d->taskEditorWidget );
    
    d->examineWidget = new ExamineWidget(d->examinator, d->stack );
    d->stack->addWidget( d->examineWidget );

    setViewMode(TogMegWindow::TaskEditorMode);

    connect(d->examinator, SIGNAL(examinatorEnabled(bool)),
            actionGroup("Play"), SLOT(setEnabled(bool)));
    connect(d->examinator, SIGNAL(stopped()),
            this, SLOT(stop()));
    connect(project()->model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            project(), SLOT(setModified()));
    
    readSettings();
}

/*!
 Deletes the object.
 */
TogMegWindow::~TogMegWindow()
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

TogMegProject * TogMegWindow::project() const
{
    return dynamic_cast<TogMegProject *>(BaseWindow::project());
}

/*!
 Returns current build number
 */
QString TogMegWindow::release() const
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
void TogMegWindow::createActions()
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
    setAction("Help"          , new QAction (tr("TogMeg &Help"), this));
    setAction("CheckUpdates" , new QAction (tr("Check for Updates Now"), this));

    setActionGroup("Play", new QActionGroup(this));
    actionGroup("Play")->setExclusive(false);
    actionGroup("Play")->addAction(action("Play"));
    actionGroup("Play")->addAction(action("Study"));
}

/*!
 Creates menubar and add actions.
 */
void TogMegWindow::createMenuBar()
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

void TogMegWindow::createToolBar()
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
void TogMegWindow::createShortcuts()
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
void TogMegWindow::connectActions()
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

void TogMegWindow::newEntry()
{
    if ( viewMode() == TogMegWindow::TaskEditorMode) {
        d->taskEditorWidget->addNewEntry();
    }
}

void TogMegWindow::previousWindow()
{
    if ( viewMode() == ExamineMode ) setViewMode(TogMegWindow::TaskEditorMode);
}

void TogMegWindow::runDemo()
{
    d->examinator->start( Examinator::Playing );
    setViewMode(TogMegWindow::ExamineMode);
}

void TogMegWindow::runStudy()
{
    d->examinator->start( Examinator::Studying );
    setViewMode(TogMegWindow::ExamineMode);
}

void TogMegWindow::stop()
{
    if ( d->examinator->state() != Examinator::Stopped ) {
        setViewMode(TogMegWindow::BrowserMode);
        d->examinator->stop();
    }
}

/*!
 * Opens default browser directed to help page
 */
void TogMegWindow::openHelp()
{
    QString url = QString("http://www.lynxline.com");
    QDesktopServices::openUrl(url);
}

void TogMegWindow::openAbout()
{
}

void TogMegWindow::switchFullScreen()
{
    if (isFullScreen()) showNormal();
    else                showFullScreen();
}

void TogMegWindow::quit()
{
    close();
    qApp->closeAllWindows();
}

TogMegWindow::ViewMode TogMegWindow::viewMode()
{
    return d->viewMode;
}

void TogMegWindow::setViewMode(TogMegWindow::ViewMode m)
{
    d->viewMode = m;
    if (m == TogMegWindow::TaskEditorMode) {
        //first switch stack
        if ( d->stack->currentWidget() != d->taskEditorWidget ) {
            d->stack->setCurrentWidget( d->taskEditorWidget );
        }
    }
    else if (m == TogMegWindow::ExamineMode) {
        //just switch stack
        d->stack->setCurrentWidget( d->examineWidget );
    }
    else if (m == TogMegWindow::BrowserMode) {
        //just switch stack
        if ( d->stack->currentWidget() != d->taskEditorWidget ) {
            d->stack->setCurrentWidget( d->taskEditorWidget );
        }

        d->viewMode = TogMegWindow::TaskEditorMode;
    }

    //actionGroup("Play")->setEnabled( d->examinator->entryCount() >0 );
    action("Play" )->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("Study")->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("Stop" )->setEnabled( d->viewMode==TogMegWindow::ExamineMode );

    emit viewModeChanged( d->viewMode );
}

QFont TogMegWindow::baseFont(qreal multiplier, int weight)
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

void TogMegWindow::openFile()
{
    QString path;
    
    path = QFileDialog::getOpenFileName(this,
                                        tr("Open a crammero file"), path,
                                        tr("Tab delimited files (*.tab);;Xml files (*.xml);;Any file (*)")
                                        );
    
    if (path.isEmpty()) return;
    
    openFile(path);
}

void TogMegWindow::openFile(QString path)
{
    qDebug() << path;
    if (path.endsWith(".xml", Qt::CaseInsensitive))
        project()->model()->loadXmlFile(path);
    else project()->model()->loadTabFile(path);
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

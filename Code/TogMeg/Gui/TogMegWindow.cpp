
#include <QtGui>
#include <QtCore>

#include "TogMegWindow.h"
#include "TogMegProject.h"

#include "TogMeg.h"
#include "Examinator.h"

#include "ExamineWidget.h"

#include "TogMegFileEdit.h"
#include "TogMegFileModel.h"

#include "FileNavigationView.h"
#include "FileNavigationModel.h"

#include "IconSet.h"
#include "AppStyles.h"
#include "PixmapButton.h"

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

class TogMegWindow::Private
{
public:
    TogMegWindow * instance;
    TogMegWindow::ViewMode viewMode;
    
    QPointer<FileNavigationModel> filesModel;
    QPointer<FileNavigationView> filesView;
    
    QPointer<Examinator> examinator;
    
    QPointer<QStackedWidget> stack;
    QPointer<ExamineWidget> examineWidget;
    QPointer<TogMegFileEdit> editor;
    
    QPointer<QDockWidget> filesDock;
    QPointer<QWidget> toolBarLeftSpacer;
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
    
    setWindowTitle("TogMeg");
    
    filesDock();
    createActions();
    connectActions();
    createMenuBar();
    createToolBar();
    updateFileMenu();

	statusBar()->hide();
    
    d->stack = new QStackedWidget(this);
    setCentralWidget(d->stack);

    d->editor = new TogMegFileEdit(project()->model(), d->stack);
    d->examineWidget = new ExamineWidget(d->examinator, d->stack );

    d->stack->addWidget( d->editor );
    d->stack->addWidget( d->examineWidget );

    connect(d->examinator, SIGNAL(examinatorEnabled(bool)),
            actionGroup("Play"), SLOT(setEnabled(bool)));
    connect(d->examinator, SIGNAL(stopped()),
            this, SLOT(stop()));
    connect(project()->model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            project(), SLOT(setModified()));
    
    readSettings();
    
    setViewMode(TogMegWindow::TaskEditorMode);
    d->editor->setNextItemMode(TogMegFileEdit::QQAAMode);
    d->editor->setFocus();    
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
    delete d->editor;

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
    BaseWindow::createActions();
    
    setAction("toolbar/New",    new QAction(EmbIcon("Add"), tr("File"), this));
    setAction("toolbar/Folder", new QAction(EmbIcon("Folder"), tr("Group"), this));
    setAction("toolbar/Remove", new QAction(EmbIcon("Remove"), tr("Remove"), this));
    setAction("Add" , new QAction(tr("&Add Row"), this));
    setAction("Swap", new QAction(tr("Swap QA"), this));

    setAction("Play"  , new QAction (EmbIcon("Play"   ), tr("&Play"), this));
    setAction("Study" , new QAction (EmbIcon("Record" ), tr("&Study"), this));
    setAction("Stop"  , new QAction (EmbIcon("Stop"   ), tr("&Stop"), this));

    setAction("NextByRows"  ,  new QAction(EmbIcon("Arrows-Ver"), tr("By Rows"), this));
    setAction("NextByCells"  , new QAction(EmbIcon("Arrows-Hor"), tr("By Cells"), this));
    action("NextByRows")->setCheckable(true);
    action("NextByCells")->setCheckable(true);
    
    setActionGroup("NextBy", new QActionGroup(this));
    actionGroup("NextBy")->setExclusive(true);
    actionGroup("NextBy")->addAction(action("NextByRows"));
    actionGroup("NextBy")->addAction(action("NextByCells"));
    action("NextByRows")->setChecked(true);
    
    action("Add")->setShortcut(QKeySequence("Alt+Down"));
    action("Swap")->setShortcut(QKeySequence("Ctrl+U"));

    setAction("About"        , new QAction(tr("&About"), this));
    setAction("Help"         , new QAction(tr("TogMeg &Help"), this));
    setAction("CheckUpdates" , new QAction(tr("Check for Updates Now"), this));

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
	menu = menuBar()->addMenu(tr("&Edit"));
    menu->addAction( action("Add") );
    menu->addAction( action("Swap") );

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
    toolBar->addAction( action("toolbar/New") );
    toolBar->addAction( action("toolbar/Folder") );
    toolBar->addAction( action("toolbar/Remove") );

    d->toolBarLeftSpacer = new QWidget;
    d->toolBarLeftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    d->toolBarLeftSpacer->setFixedHeight(0);
    toolBar->addWidget(d->toolBarLeftSpacer);

    /*
    QWidget * sp1 = new QWidget;
    sp1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    sp1->setFixedWidth(40);
    sp1->setFixedHeight(0);
    toolBar->addWidget(sp1);
    */
    
	toolBar->addAction( action("Play") );
	toolBar->addAction( action("Study") );
    toolBar->addAction( action("Stop") );

    QWidget * sp_exp = new QWidget;
    sp_exp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    sp_exp->setFixedHeight(0);
    toolBar->addWidget(sp_exp);

    toolBar->addAction( action("NextByRows") );
    toolBar->addAction( action("NextByCells") );

    toolBar->setIconSize(QSize(24, 24));
    toolBar->setMovable(false);
    
#ifdef Q_WS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
    toolBar->setStyleSheet(AppStyles::mac_toolbarStyle);
#endif
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

/*!
 Connects actions to appropriate slots.
 */
void TogMegWindow::connectActions()
{
    connect( action("Help"),   SIGNAL(triggered()), this, SLOT(openHelp()));
    connect( action("About"),  SIGNAL(triggered()), this, SLOT(openAbout()));

    connect( action("toolbar/New"),    SIGNAL(triggered()), this, SLOT(newFile()));
    connect( action("toolbar/Folder"), SIGNAL(triggered()), d->filesView, SLOT(addItem()));
    connect( action("toolbar/Remove"), SIGNAL(triggered()), d->filesView, SLOT(removeItem()));

    connect( action("Add"),    SIGNAL(triggered()), this, SLOT(newEntry()));
    connect( action("Swap"),    SIGNAL(triggered()), this, SLOT(swapQA()));
    connect( action("NextByRows"),   SIGNAL(triggered()), this, SLOT(setNextByRows()));
    connect( action("NextByCells"),   SIGNAL(triggered()), this, SLOT(setNextByCells()));

    connect( action("Play" ),  SIGNAL(triggered()), this, SLOT(runDemo()));
    connect( action("Study"),  SIGNAL(triggered()), this, SLOT(runStudy()));
    connect( action("Stop" ),  SIGNAL(triggered()), this, SLOT(stop()));
}

void TogMegWindow::newEntry()
{
    if ( viewMode() == TogMegWindow::TaskEditorMode) {
        d->editor->addNewEntry();
    }
}

void TogMegWindow::swapQA()
{
    if ( viewMode() == TogMegWindow::TaskEditorMode) {
        d->editor->swapQA();
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
    setViewMode(TogMegWindow::BrowserMode);
    if ( d->examinator->state() != Examinator::Stopped ) {
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
        if ( d->stack->currentWidget() != d->editor ) {
            d->stack->setCurrentWidget( d->editor );
            d->filesDock->setVisible(false);
        }
    }
    else if (m == TogMegWindow::ExamineMode) {
        //just switch stack
        d->stack->setCurrentWidget( d->examineWidget );
        d->filesDock->setVisible(false);
    }
    else if (m == TogMegWindow::BrowserMode) {
        //just switch stack
        if ( d->stack->currentWidget() != d->editor ) {
            d->stack->setCurrentWidget( d->editor );
            if (!d->filesDock->isVisible())
                d->filesDock->setVisible(true);
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

QDockWidget * TogMegWindow::filesDock() const
{    
    if (!d->filesDock) {
        QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
        d->filesModel = new FileNavigationModel(d->instance);
        d->filesModel->setRootPath(path);

        d->filesView = new FileNavigationView;
        d->filesView->setModel(d->filesModel);
        connect(d->filesView, SIGNAL(widthChanged(int)),
                this, SLOT(adjustSpacerInToolBar(int)));
        connect(d->filesView, SIGNAL(openFileRequest(QString)),
                this, SLOT(openFile(QString)));
        
        QWidget * filesWidget = new QWidget;
        QVBoxLayout * filesLayout = new QVBoxLayout;
        filesLayout->setSpacing(0);
        filesLayout->setMargin(0);
        filesWidget->setLayout(filesLayout);
        
        filesLayout->addWidget(d->filesView);
        
        d->filesDock = new QDockWidget(d->instance);
        d->filesDock->setMinimumWidth(200);
        d->filesDock->setWidget(filesWidget);
        d->filesDock->setObjectName("Files");
        d->filesDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
        d->instance->addDockWidget(Qt::LeftDockWidgetArea, d->filesDock);
        
        QWidget * title = new QWidget;
        title->setFixedHeight(0);
        d->filesDock->setTitleBarWidget(title);
    }
    
    return d->filesDock;
}

void TogMegWindow::adjustSpacerInToolBar(int /*w*/)
{
    //d->toolBarLeftSpacer->setFixedWidth(w -160);
}

void TogMegWindow::setNextByRows()
{
    d->editor->setNextItemMode(TogMegFileEdit::QQAAMode);
}

void TogMegWindow::setNextByCells()
{
    d->editor->setNextItemMode(TogMegFileEdit::QAQAMode);
}

void TogMegWindow::newFile()
{
    QString fileName = project()->fileName();
    if (fileName.isEmpty()) {
        if (!allowToClose())
            return;
    }
    else
        saveFile();

    stop();
    project()->clear();
    updateWindowTitle();
    
    setViewMode(TogMegWindow::TaskEditorMode);
    d->editor->setFocus();
    d->editor->setCurrentIndex(d->editor->model()->index(1,0));

    if (project()->isModified())
        saveAsFile();
}

bool TogMegWindow::openFile(QString path)
{
    if (project()->isModified()) {
        QString fileName = project()->fileName();
        if (fileName.isEmpty()) {
            if (!allowToClose()) {
                metaObject()->invokeMethod(d->filesView, "clearSelection", Qt::QueuedConnection);
                return false;
            }
        }
        else
            saveFile();
    }

    project()->setModified(false);
    return BaseWindow::openFile(path);
}

void TogMegWindow::closeEvent(QCloseEvent * event)
{
    if (project()->isModified()) {
        QString fileName = project()->fileName();
        if (fileName.isEmpty()) {
            if (!allowToClose())
                return;
        }
        else
            saveFile();
    }

    saveSettings();
    event->accept();
}

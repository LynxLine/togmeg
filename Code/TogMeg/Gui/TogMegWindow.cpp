
#include <QtGui>
#include <QtCore>

#include "TogMegWindow.h"
#include "TogMegProject.h"

#include "TogMeg.h"
#include "Examinator.h"

#include "ExamineWidget.h"

#include "QAEditor.h"
#include "QAModel.h"

#include "FileNavigationView.h"
#include "FileNavigationModel.h"

#include "IconSet.h"
#include "MiscUtils.h"
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
    QPointer<QAEditor> editor;
    
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
    //setWindowIcon(EmbIcon("TogMeg"));
    
    filesDock();
    createActions();
    connectActions();
    createMenuBar();
    createToolBar();
    updateFileMenu();

	statusBar()->hide();
    
    d->stack = new QStackedWidget(this);
    setCentralWidget(d->stack);

    d->editor = new QAEditor(project()->model(), d->stack);
    d->examineWidget = new ExamineWidget(d->examinator, d->stack );

    connect(action("Cut"), SIGNAL(triggered()), d->editor, SLOT(cut()));
    connect(action("Copy"), SIGNAL(triggered()), d->editor, SLOT(copy()));
    connect(action("Paste"), SIGNAL(triggered()), d->editor, SLOT(paste()));

    d->stack->addWidget( d->editor );
    d->stack->addWidget( d->examineWidget );

    connect(d->examinator, SIGNAL(examinatorEnabled(bool)),
            actionGroup("Play"), SLOT(setEnabled(bool)));
    connect(d->examinator, SIGNAL(stopped()),
            this, SLOT(stop()));
    connect(project()->model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            project(), SLOT(setModified()));
    
    readSettings();
    d->filesDock->setVisible(true);

    setViewMode(TogMegWindow::TaskEditorMode);
    d->editor->setMode(QAEditor::QQAAMode);
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
    
    setAction("toolbar/New",    new QAction(EmbIcon("Add"), tr("New"), this));
    setAction("toolbar/Folder", new QAction(EmbIcon("Folder"), tr("Group"), this));
    setAction("toolbar/Remove", new QAction(EmbIcon("Remove"), tr("Remove"), this));
    setAction("Add" , new QAction(tr("&Add Row"), this));
    setAction("Swap", new QAction(tr("Swap QA"), this));

    setAction("Cut" ,  new QAction(tr("Cut"), this));
    setAction("Copy",  new QAction(tr("Copy"), this));
    setAction("Paste", new QAction(tr("Paste"), this));
    setAction("Undo",  new QAction(tr("Undo"), this));
    setAction("Redo",  new QAction(tr("Redo"), this));
    setAction("SelectAll", new QAction(tr("SelectAll"), this));

    setAction("Play"  , new QAction(tr("&Play"), this));
    setAction("Study" , new QAction(tr("&Study"), this));
    setAction("Stop"  , new QAction(tr("&Stop"), this));

    setAction("toolbar/Play"  , new QAction(EmbIcon("Play"   ), tr("&Play"), this));
    setAction("toolbar/Study" , new QAction(EmbIcon("Record" ), tr("&Study"), this));
    setAction("toolbar/Stop"  , new QAction(EmbIcon("Stop"   ), tr("&Stop"), this));

    setAction("toolbar/NextByRows"  ,  new QAction(EmbIcon("Arrows-Ver"), tr("Rows"), this));
    setAction("toolbar/NextByCells"  , new QAction(EmbIcon("Arrows-Hor"), tr("Cells"), this));
    action("toolbar/NextByRows")->setCheckable(true);
    action("toolbar/NextByCells")->setCheckable(true);
    
    setActionGroup("NextBy", new QActionGroup(this));
    actionGroup("NextBy")->setExclusive(true);
    actionGroup("NextBy")->addAction(action("toolbar/NextByRows"));
    actionGroup("NextBy")->addAction(action("toolbar/NextByCells"));
    action("toolbar/NextByRows")->setChecked(true);
    
    action("toolbar/Remove")->setEnabled(false);
    action("Add")->setShortcut(QKeySequence("Alt+Down"));
    action("Swap")->setShortcut(QKeySequence("Ctrl+U"));
    action("Cut")->setShortcut(QKeySequence::Cut);
    action("Copy")->setShortcut(QKeySequence::Copy);
    action("Paste")->setShortcut(QKeySequence::Paste);
    action("Undo")->setShortcut(QKeySequence::Undo);
    action("Redo")->setShortcut(QKeySequence::Redo);
    action("SelectAll")->setShortcut(QKeySequence::SelectAll);

    action("Play")->setShortcut(QKeySequence("Ctrl+B"));
    action("Study")->setShortcut(QKeySequence("Ctrl+R"));
    action("Stop")->setShortcut(QKeySequence("Esc"));

    setAction("About"        , new QAction(tr("&About"), this));
    setAction("Help"         , new QAction(tr("TogMeg &Help"), this));
    setAction("CheckUpdates" , new QAction(tr("Check for Updates Now"), this));

    setActionGroup("Play", new QActionGroup(this));
    actionGroup("Play")->setExclusive(false);
    actionGroup("Play")->addAction(action("toolbar/Play"));
    actionGroup("Play")->addAction(action("toolbar/Study"));
}

/*!
 Creates menubar and add actions.
 */
void TogMegWindow::createMenuBar()
{
    QMenu * menu;
	menu = menuBar()->addMenu(tr("&Edit"));
    //menu->addAction( action("Undo") );
    //menu->addAction( action("Redo") );
    //menu->addSeparator();
    menu->addAction( action("Cut") );
    menu->addAction( action("Copy") );
    menu->addAction( action("Paste") );
    menu->addSeparator();
    menu->addAction( action("Add") );
    menu->addAction( action("Swap") );

	menu = menuBar()->addMenu(tr("&Run"));
	menu->addAction( action("Play") );
	menu->addAction( action("Study") );
	menu->addSeparator();
	menu->addAction( action("Stop") );

	menu = menuBar()->addMenu(tr("&Window"));
	menu->addAction( action("window/Minimize") );
    menu->addAction( action("window/Zoom") );

    menu = menuBar()->addMenu(tr("&Help"));
	menu->addAction( action("About") );
    //menu->addSeparator();
    //menu->addAction( action("CheckUpdates") );
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
    
	toolBar->addAction( action("toolbar/Play") );
	toolBar->addAction( action("toolbar/Study") );
    toolBar->addAction( action("toolbar/Stop") );

    QWidget * sp_exp = new QWidget;
    sp_exp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    sp_exp->setFixedHeight(0);
    toolBar->addWidget(sp_exp);

    QWidget * sp_exp24 = new QWidget;
    sp_exp24->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    sp_exp24->setFixedWidth(40);
    sp_exp24->setFixedHeight(0);
    toolBar->addWidget(sp_exp24);

    toolBar->addAction( action("toolbar/NextByRows") );
    toolBar->addAction( action("toolbar/NextByCells") );

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
    connect( action("toolbar/NextByRows"),   SIGNAL(triggered()), this, SLOT(setNextByRows()));
    connect( action("toolbar/NextByCells"),   SIGNAL(triggered()), this, SLOT(setNextByCells()));

    connect( action("Play" ),  SIGNAL(triggered()), this, SLOT(runDemo()));
    connect( action("Study"),  SIGNAL(triggered()), this, SLOT(runStudy()));
    connect( action("Stop" ),  SIGNAL(triggered()), this, SLOT(stop()));

    connect( action("toolbar/Play" ),  SIGNAL(triggered()), this, SLOT(runDemo()));
    connect( action("toolbar/Study"),  SIGNAL(triggered()), this, SLOT(runStudy()));
    connect( action("toolbar/Stop" ),  SIGNAL(triggered()), this, SLOT(stop()));
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

    action("Play" )->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("Study")->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("Stop" )->setEnabled( d->viewMode==TogMegWindow::ExamineMode );

    action("toolbar/New"   )->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("toolbar/Folder")->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("toolbar/Remove")->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    checkRemoveAvailable();

    action("toolbar/Play" )->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("toolbar/Study")->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("toolbar/Stop" )->setEnabled( d->viewMode==TogMegWindow::ExamineMode );

    action("toolbar/NextByRows" )->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );
    action("toolbar/NextByCells")->setEnabled( d->viewMode!=TogMegWindow::ExamineMode );

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
        if (!path.endsWith("/")) path += "/";
        path += "Library";

        d->filesModel = new FileNavigationModel(d->instance);
        d->filesModel->setRootPath(path);

        d->filesView = new FileNavigationView;
        d->filesView->setModel(d->filesModel);
        connect(d->filesView, SIGNAL(widthChanged(int)),
                this, SLOT(adjustSpacerInToolBar(int)));
        connect(d->filesView, SIGNAL(openFileRequest(QString)),
                this, SLOT(openFile(QString)));
        connect(d->filesView->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(checkRemoveAvailable()));
        
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
        d->filesDock->setVisible(true);
    }
    
    return d->filesDock;
}

void TogMegWindow::adjustSpacerInToolBar(int /*w*/)
{
    //d->toolBarLeftSpacer->setFixedWidth(w -160);
}

void TogMegWindow::setNextByRows()
{
    d->editor->setMode(QAEditor::QQAAMode);
}

void TogMegWindow::setNextByCells()
{
    d->editor->setMode(QAEditor::QAQAMode);
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

bool TogMegWindow::saveAsFile()
{
    QString path = project()->filePath();
    if (path.isEmpty())
        QDir::setCurrent(d->filesModel->path());

    return BaseWindow::saveAsFile();
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

void TogMegWindow::checkRemoveAvailable()
{
    QSet<QString> names;
    foreach(QModelIndex mi, d->filesView->selectionModel()->selectedIndexes()) {
        QString name = mi.data().toString();
        if (name == "." || name == "..") continue;
        names << name;
    }
    action("toolbar/Remove")->setEnabled(names.count() > 0);
}

void TogMegWindow::openAbout()
{
    QString htmlText =
            tr("<HTML>"
               "<p>is simple application to make the process"\
               " of studying new words of foreign languages very efficient."\
               " You can use Text-To-Speech engines and follow the"\
               " phonation and pronouncing of the languages."
               " </p>"
               "</HTML>");

    QMessageBox box(this);
    box.setWindowFlags(Qt::Dialog |
                       Qt::CustomizeWindowHint
                       );
    box.setWindowTitle("About " + qApp->applicationName());
    box.setTextFormat(Qt::RichText);
    box.setText(tr("TogMeg application, version 1.1, "\
                   "build %1").arg(MiscUtils::buildId())
                .replace(" ", "&nbsp;"));
    box.setInformativeText(htmlText);
    box.setIconPixmap(EmbPixmap("TogMeg").scaled(72,72,
                                                 Qt::IgnoreAspectRatio,
                                                 Qt::SmoothTransformation));
    box.exec();
}

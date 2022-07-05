//
// Copyright Oleksandr Iakovliev, 2010
//

#include <QtGui>
#include "BaseWindow.h"
#include "BaseProject.h"

#include "IconSet.h"
#include "AppStyles.h"
#include "MiscUtils.h"

#define S_WINDOWGEOM    "/Geometry"
#define S_WINDOWSTATE   "/WindowState"
#define S_RECENTFILES    "/RecentFiles"

class BaseWindow::Private {
public:
    QPointer<QMenu> fileMenu;
    QPointer<QMenu> recentMenu;
    QPointer<BaseWindow> instance;
    QPointer<BaseProject> project;
    QMap<QString, QAction *> actions;
    QMap<QString, QActionGroup *> actionGroups;  
};

BaseWindow::BaseWindow(BaseProject * proj, QWidget * parent, Qt::WFlags flags)
:QMainWindow(parent, flags)
{
    d = new Private;
    d->instance = this;
    d->project = proj;
    
    connect(project(), SIGNAL(loadCompleted()), this, SLOT(updateRecentList()));
    connect(project(), SIGNAL(saveCompleted()), this, SLOT(updateRecentList()));
    connect(project(), SIGNAL(modifiedStatusChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(project(), SIGNAL(filePathChanged(const QString &)), 
            this,        SLOT(updateWindowTitle()));

    d->fileMenu = menuBar()->addMenu(tr("&File"));
    d->recentMenu = d->fileMenu->addMenu(tr("Open Recent"));

    connect(d->fileMenu, SIGNAL(aboutToShow()), this, SLOT(updateFileMenu()));
    connect(d->recentMenu, SIGNAL(aboutToShow()), this, SLOT(updateRecentMenu()));

    setObjectName(metaObject()->className());    
    updateWindowTitle();
}

BaseWindow::~BaseWindow()
{
    delete d;
}

QAction * BaseWindow::action(QString name) const
{
    if (!d->actions.contains(name)) {
        qDebug() << "BaseWindow::action(), not found:" << name;
        return 0L;
    }
    
    return d->actions[name];
}

QActionGroup * BaseWindow::actionGroup(QString name) const
{
    if (!d->actionGroups.contains(name))
        return 0L;
    
    return d->actionGroups[name];
}

void BaseWindow::setAction(QString name, QAction * a)
{
    d->actions[name] = a;
}

void BaseWindow::setActionGroup(QString name, QActionGroup * ag)
{
    d->actionGroups[name] = ag;
}

void BaseWindow::createActions()
{
    setAction("New"       , new QAction(tr("&New"), this));
    setAction("Open"      , new QAction(tr("&Open..."), this));
    setAction("Revert"    , new QAction(tr("Revert to Saved"), this));    
    setAction("Save"      , new QAction(tr("&Save"), this));
    setAction("SaveAs"    , new QAction(tr("Save &As..."), this));
    setAction("Close"     , new QAction(tr("&Close"), this));
    setAction("Quit"      , new QAction(tr("&Quit"), this));
    setAction("ClearRecentMenu", new QAction(tr("&Clear Menu"), this));

    setAction("window/Minimize" , new QAction(tr("Minimize"), this));
    setAction("window/Zoom"     , new QAction(tr("Zoom"), this));

    action("New")->setShortcut(QKeySequence::New);
    action("Open")->setShortcut(QKeySequence::Open);
    action("Save")->setShortcut(QKeySequence::Save);
    action("SaveAs")->setShortcut(tr("Ctrl+Shift+S"));
    action("Revert")->setShortcut(tr("Ctrl+Shift+R"));
    action("Close")->setShortcut(tr("Ctrl+W"));
    action("Quit")->setShortcut(tr("Ctrl+Q"));
    action("window/Minimize")->setShortcut(tr("Ctrl+M"));
    
    action("New")->setStatusTip(tr("Create a new file"));
    action("Open")->setStatusTip(tr("Open an existing file"));
    action("Save")->setStatusTip(tr("Save the document to disk"));
    action("SaveAs")->setStatusTip(tr("Save the document to disk"));
    action("Quit")->setStatusTip(tr("Quit the application"));
    action("window/Minimize")->setStatusTip(tr("Minimize current window"));
    
    connect(action("New")   , SIGNAL(triggered()), this, SLOT(newFile()));
    connect(action("Open")  , SIGNAL(triggered()), this, SLOT(openFile()));
    connect(action("Save")  , SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(action("SaveAs"), SIGNAL(triggered()), this, SLOT(saveAsFile()));
    connect(action("Revert"), SIGNAL(triggered()), this, SLOT(revertToSaved()));
    connect(action("Close") , SIGNAL(triggered()), this, SLOT(close()));
    connect(action("Quit")  , SIGNAL(triggered()), this, SLOT(close()));
    connect(action("ClearRecentMenu"), SIGNAL(triggered()), this, SLOT(clearRecentMenu()));
    connect(action("window/Minimize"), SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(action("window/Zoom"),     SIGNAL(triggered()), this, SLOT(showWindowZoomed()));
}

QMenu * BaseWindow::fileMenu() const {
    return d->fileMenu;
}

QMenu * BaseWindow::recentMenu() const {
    return d->recentMenu;
}

void BaseWindow::updateFileMenu()
{
    d->fileMenu->clear();
    d->fileMenu->addAction(action("New"));
    d->fileMenu->addAction(action("Open"));
    d->fileMenu->addMenu(d->recentMenu);
    d->fileMenu->addSeparator();
    d->fileMenu->addAction(action("Close"));
    d->fileMenu->addAction(action("Save"));
    d->fileMenu->addAction(action("SaveAs"));    
    d->fileMenu->addAction(action("Revert"));
#ifndef Q_WS_MAC
    d->fileMenu->addSeparator();
    d->fileMenu->addAction(action("Quit"));
#endif
}

void BaseWindow::clearRecentMenu()
{
    QSettings s;
    QString prefix = metaObject()->className();
    s.setValue(prefix+S_RECENTFILES, QStringList());
}

void BaseWindow::updateRecentList()
{
    QString filePath = project()->filePath();
    if (filePath.isEmpty()) return;
    
    QSettings s;
    QString prefix = metaObject()->className();
    QStringList recentFiles = s.value(prefix+S_RECENTFILES).toStringList();
    
    recentFiles.removeAll(filePath);
    recentFiles.prepend(filePath);
    
    s.setValue(prefix+S_RECENTFILES, recentFiles);
    updateWindowTitle();
}

void BaseWindow::updateRecentMenu()
{
    recentMenu()->clear();
    
    QSettings s;
    QString prefix = metaObject()->className();
    QStringList recentFilePaths = s.value(prefix+S_RECENTFILES).toStringList();
    int recentFilesCount = qMin(recentFilePaths.count(), 9);
    
    QStringList filePaths;
    QStringList fileNames;
    
    for (int i = 0; i < recentFilesCount; i++) {
        QFileInfo fi(recentFilePaths[i]);
        if (!fi.exists()) continue;
        
        filePaths << recentFilePaths[i];
        fileNames << fi.fileName();
    }        
    
    for (int i = 0; i < filePaths.count(); i++) {
        QString filePath = filePaths[i];
        QString visibleName = fileNames[i];
        
        QString text = tr("&%1. %2").arg( i + 1 ).arg( visibleName );
        text = QDir::toNativeSeparators(text);        

        QAction * a = recentMenu()->addAction(text);
        a->setData( filePath );
        a->setStatusTip( filePath );
        connect(a, SIGNAL(triggered()), this, SLOT(openRecentFileActivated()));
    }
    
    if (filePaths.count()) {
        recentMenu()->addSeparator();
        recentMenu()->addAction(action("ClearRecentMenu"));
    }
}

bool BaseWindow::openRecentFileActivated()
{
    QAction * a = dynamic_cast<QAction *>(sender());
    if (!a) return false;
    
    QString filePath = a->data().toString();
    if (filePath.isEmpty()) return false;
    
    if (allowToClose())
        return project()->loadFile(filePath);
    
    return false;
}

BaseProject * BaseWindow::project() const
{
    return d->project;
}

bool BaseWindow::allowToClose()
{
    if (!project()->isModified())
        return true;
    
    QString fileName = project()->fileName();
    if (fileName.isEmpty()) fileName = "Untitled";
    
    QMessageBox box(QMessageBox::Information, tr("Save File?"),
                    tr("Do you want to save the changes to \"%1\" before closing?").arg(fileName),
                    QMessageBox::Discard | QMessageBox::Cancel | QMessageBox::Save, this);
    box.setInformativeText(tr("If you don't save, your changes will be lost."));
    box.setWindowModality(Qt::WindowModal);
    box.setDefaultButton(QMessageBox::Save);
    
    static_cast<QPushButton *>(box.button(QMessageBox::Discard))->setMinimumWidth(90);
    static_cast<QPushButton *>(box.button(QMessageBox::Cancel))->setMinimumWidth(90);
    static_cast<QPushButton *>(box.button(QMessageBox::Save))->setMinimumWidth(90);
    
#ifdef Q_WS_WIN
    QString text = box.text();
    box.setText( tr("<font size='+1'>%1</font>").arg(text) );
    static_cast<QPushButton *>(box.button(QMessageBox::Discard))->setText(tr("Don't Save"));
    static_cast<QPushButton *>(box.button(QMessageBox::Save))->setText(tr("Save"));
#endif
    
    switch (box.exec()) {
        case QMessageBox::Save: {
            return saveFile();
        }
        case QMessageBox::Discard: {
            break;
        }
        case QMessageBox::Cancel:
            return false;
    }
    
    return true;
}

void BaseWindow::closeEvent(QCloseEvent * event)
{
    if (!allowToClose())
        return event->ignore();
    
    saveSettings();
    event->accept();
}

/*!
 Loads and apply settings of MainWindow such as geometry, position, state,
 maximized/minimized/normal, state of docking widgets etc.
 */
void BaseWindow::readSettings()
{
    QSettings s;
    QString prefix = metaObject()->className();
    QByteArray geom = s.value(prefix+S_WINDOWGEOM).toByteArray();
    QByteArray state = s.value(prefix+S_WINDOWSTATE).toByteArray();
	
    if (!state.isEmpty())
        restoreState(state);

    if (!geom.isEmpty())
		restoreGeometry(geom);
	else resize(900, 600);    
}

/*!
 */
void BaseWindow::saveSettings() const
{
    QByteArray geom = saveGeometry();
    QByteArray state = saveState();
    
    QSettings s;
    QString prefix = metaObject()->className();
    s.setValue(prefix+S_WINDOWGEOM, geom);
    s.setValue(prefix+S_WINDOWSTATE, state);
}

bool BaseWindow::openFile(QString path)
{
    if (!allowToClose())
        return false;
    
    if (path.isEmpty()) {
        QString title = tr("Open a %1 file").arg(project()->extension().toUpper());

        path = project()->filePath();
        if (path.isEmpty())
            path = QDir().absolutePath();
        else path = project()->folderPath();

#ifdef Q_WS_MAC
        QFileDialog dlg(this,Qt::Sheet);
        dlg.setNameFilter(project()->fileDialogFilter());
        dlg.setFileMode(QFileDialog::ExistingFile);
        dlg.setWindowTitle(title);
        dlg.setDirectory(path);

        QEventLoop el;
        connect(&dlg, SIGNAL(finished(int)), &el, SLOT(quit()));
        dlg.show();
        el.exec();

        if (dlg.result() != QDialog::Accepted)
            return false;

        if (!dlg.selectedFiles().count())
            return false;

        path = dlg.selectedFiles().first();
#else
        path = QFileDialog::getOpenFileName(this, title, path,  
                                            project()->fileDialogFilter());
#endif
    }

    if (path.isEmpty())
        return false;
    
    bool ok = project()->loadFile(path);    
    if (!ok) {
        QString fileName = QFileInfo(path).fileName();
        QMessageBox box(QMessageBox::Information, tr("Load is failed"),
                        tr("%1: load is failed").arg(fileName),
                        QMessageBox::Ok, this);
        box.setInformativeText(tr("Content of the file may be wrong."));
        box.setWindowModality(Qt::WindowModal);
        box.setDefaultButton(QMessageBox::Ok);
        
#ifdef Q_WS_WIN
        QString text = box.text();
        box.setText( tr("<font size='+1'>%1</font>").arg(text) );
#endif
        box.exec();
    }
    
    return ok;
}

bool BaseWindow::saveAsFile()
{
    QString title = tr("Save a %1 file").arg(project()->extension().toUpper());
    QString path = project()->filePath();

    if (path.isEmpty())
        path = QDir(project()->folderPath()).absoluteFilePath("Untitled");

#ifdef Q_WS_MAC
    QFileDialog dlg(this,Qt::Sheet);
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setNameFilter(project()->fileDialogFilter());
    dlg.setWindowTitle(title);
    dlg.setDirectory(path);
    dlg.selectFile(path);

    QEventLoop el;
    dlg.open(&el, SLOT(quit()));
    connect(&dlg, SIGNAL(finished(int)), &el, SLOT(quit()));
    dlg.show();
    el.exec();

    if (dlg.result() != QDialog::Accepted)
        return false;

    if (!dlg.selectedFiles().count())
        return false;

    path = dlg.selectedFiles().first();
#else
    path = QFileDialog::getSaveFileName(this, title, path, 
                                        project()->fileDialogFilter()); 	 	     
#endif

    if (path.isEmpty())
        return false;

    bool haveExtension = false;
    foreach(QString ext, project()->extensions()) {
        if (path.endsWith("."+ext, Qt::CaseInsensitive))
            haveExtension = true;
    }

    if (!haveExtension)
        path += "."+project()->extension();

    return project()->saveFile(path);
}

bool BaseWindow::saveFile(QString path)
{
    if (path.isEmpty())
        path = project()->filePath();

    if (path.isEmpty())
        return saveAsFile();
    
    return project()->saveFile(path);
}

bool BaseWindow::revertToSaved()
{
    if (project()->filePath().isEmpty())
        return false;
    
    return project()->loadFile(project()->filePath());
}

/*!
 If window in normal state, shows it maximized,
 otherwise showNormal() called.
 */
void BaseWindow::showWindowZoomed()
{
    if (isMaximized()) 
        showNormal();
    else showMaximized();
}

void BaseWindow::updateWindowTitle()
{
    if (!project()->fileName().isEmpty()) {
        setWindowTitle("[*] " + project()->fileName());
    }
    else setWindowTitle("[*] Untitled");
    setWindowFilePath(project()->fileName());
}

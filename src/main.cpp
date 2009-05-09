//
// Copyright (C) 2007 CafeScribe
//

#include <QtGui>
#include <iostream>

#include "crammero.h"
#include "mainwindow.h"

void initResources();

int main( int argc, char ** argv )
{
    initResources();
    QApplication a(argc, argv );

    QPixmap pmSplash(":/images/Splash.png");
    QSplashScreen * splash = new QSplashScreen(pmSplash);
    splash->show();

    a.connect(&a, SIGNAL(lastWindowClosed()), 
              &a,   SLOT(quit()) );

#ifndef Q_WS_MAC
    QIcon icon(":/images/crammeroicon.png");
    icon.addFile(":/images/crammeroicon.png", QSize(128, 128));
    a.setWindowIcon( icon );
#endif

    // qsettings stuff
    QCoreApplication::setOrganizationName("LynxLine");
    QCoreApplication::setOrganizationDomain("lynxline.com");
    QCoreApplication::setApplicationName("crammero");

    //register types
    qRegisterMetaType<QModelIndex>("QModelIndex");

    // prepare storage
    QDir storageDir;
    storageDir.mkpath(app::storagePath());

    //main window
    MainWindow * mw = new MainWindow;

    QSettings s;
    bool maximized = s.value("geometry/maximized", false).toBool();
    bool fullscreen = s.value("geometry/fullscreen", false).toBool();

    if (fullscreen)     mw->showFullScreen();
    else if (maximized) mw->showMaximized();
    else                mw->show();

    splash->finish(mw);
    delete splash;

    int result = a.exec();

    // remove all widgets
    delete mw;

    return result;
}

void initResources()
{
    //init images
    Q_INIT_RESOURCE(images);
}

//
// Copyright (C) 2007 CafeScribe
//

#include <QtGui>
#include "QtSingleApplication"
#include <iostream>

#include "logger.h"
#include "crammero.h"
#include "mainwindow.h"

void initResources();
void messageHandler(QtMsgType type, const char * msg);

int main( int argc, char ** argv )
{
    //set debug handler to our handler
    qInstallMsgHandler(messageHandler);

    initResources();
    QtSingleApplication a("crammero", argc, argv );

    for (int j=0; j<argc; j++) {
        if ( QString( argv[j] ) == "/stop" ) {
            a.sendMessage("Stop");
            return 0;
        }
    }

    if (a.sendMessage("Restore")) return 0;

    a.initialize();
    new Logger(&a);

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

    a.connect(&a, SIGNAL(messageReceived(const QString &)),
              mw,   SLOT(messageReceived(const QString &)));

    splash->finish(mw);
    delete splash;

    int result = a.exec();

    // remove all widgets
    delete mw;

    return result;
}

/*!
 * This is method is registered as the message Handler for Qt
 * debug, warning, critcal and fatal errors.
 */
void messageHandler(QtMsgType type, const char *msg)
{
    if (Logger::instance())
        Logger::instance()->messageHandler(type, msg);
    else
        fprintf(stderr, "%s\n", msg);

    if (type == QtFatalMsg)
        abort();
}

void initResources()
{
    //init images
    Q_INIT_RESOURCE(images);
}

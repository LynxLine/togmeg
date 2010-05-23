//
// Copyright (C) 2007 CafeScribe
//

#include <QtGui>
#include <iostream>

#include "TogMeg.h"
#include "TogMegWindow.h"
#include "TogMegProject.h"

void initResources();

int main( int argc, char ** argv )
{
    initResources();
    Application a(argc, argv );

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

    //main window
    TogMegProject * p = new TogMegProject(&a);
    TogMegWindow mw(p);

    QSettings s;
    bool maximized = s.value("geometry/maximized", false).toBool();
    bool fullscreen = s.value("geometry/fullscreen", false).toBool();

    if (fullscreen)     mw.showFullScreen();
    else if (maximized) mw.showMaximized();
    else                mw.show();

    a.connect(&a, SIGNAL(fileOpenRequest(QString)),
              &mw,  SLOT(openFile(QString)));
    a.connect(&a, SIGNAL(lastWindowClosed()), 
              &a,   SLOT(quit()) );

    if (argc > 1) {
        for (int i =1; i< argc; i++)
            mw.openFile(argv[i]);
    }

    return a.exec();
}

void initResources()
{
    //init images
    Q_INIT_RESOURCE(images);
}

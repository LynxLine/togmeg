
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
    QCoreApplication::setApplicationName("TagMeg");

    //register types
    qRegisterMetaType<QModelIndex>("QModelIndex");

    //main window
    TogMegProject * p = new TogMegProject(&a);
    a.connect(&a, SIGNAL(fileOpenRequest(QString)),
              p,  SLOT(loadFile(QString)));

    if (argc > 1) {
        for (int i =1; i< argc; i++) {
            if (QFileInfo(argv[i]).exists())
                p->loadFile(argv[i]);
        }
    }
    
    TogMegWindow mw(p);

    QSettings s;
    bool maximized = s.value("geometry/maximized", false).toBool();
    bool fullscreen = s.value("geometry/fullscreen", false).toBool();
    
    if (fullscreen)     mw.showFullScreen();
    else if (maximized) mw.showMaximized();
    else                mw.show();
    
    return a.exec();
}

void initResources()
{
    //init images
    Q_INIT_RESOURCE(Icons);
    Q_INIT_RESOURCE(images);
}
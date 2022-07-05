
#include <QtGui>

#include "mainwindow.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    MainWindow mainWindow;
    mainWindow.show();
    
    int result = a.exec();
    return result;
}



#include <QtGui>

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    int result = a.exec();
    return result;
}


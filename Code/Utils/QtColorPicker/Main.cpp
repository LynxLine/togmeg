
#include <QtGui>
#include <QtColorPicker>

int main(int argc, char *argv[])
{
    // app object
    QApplication app(argc, argv);
    
    QMainWindow mw;
    mw.setUnifiedTitleAndToolBarOnMac(true);
    QToolBar * tb = mw.addToolBar("Test");
    QtColorPicker * cp = new QtColorPicker;
    cp->setStandardColors();
    tb->addWidget(cp);
    mw.resize(500,300);
    mw.show();

    return app.exec();
}

//
// Copyright Petrostreamz 2007
//

#include <QtGui>
#include "AppStyles.h"

#ifdef Q_WS_MAC
    #include <Carbon/Carbon.h>
    extern QString qt_mac_from_pascal_string(const Str255); //qglobal.cpp
#endif

const QString AppStyles::mac_toolbarStyle = "\
    QToolBar { margin: 0px; padding: 0px; spacing 3px; border: 0px; } \
    QToolButton { border: 1px solid rgba(0,0,0,0); border-top: 0px; border-bottom: 0px; } \
    QToolButton:checked {\
        background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
            stop: 0 rgba(0,0,0,0), stop: 0.5 rgba(0,0,0,39), stop: 1 rgba(0,0,0,0));\
        border: 1px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
            stop: 0 rgba(0,0,0,0), stop: 0.5 rgba(0,0,0,128), stop: 1 rgba(0,0,0,0)); \
        border-top: 0px; \
        border-bottom: 0px; \
    }";

const QString AppStyles::win_toolbarStyle = "\
    QToolBar { background-color: #f8f8f8; \
        margin: 0px; padding: 0px; spacing: 10px; \
        border: 1px solid #808080; \
        border-top: 0px; \
        border-left: 0px; \
        border-right: 0px; \
    } \
    QToolButton { \
        min-width: 40px; \
        border: 1px solid #808080; \
        border-top: 0px; \
        border-left: 0px; \
        border-right: 0px; \
    } \
    QToolButton:checked {\
        background-color: #c0d0e8;\
    }";

const QString AppStyles::mac_horHeaderStyle = "\
    QHeaderView {\
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
            stop:0 #ffffff, stop:0.5 #e0e0e0, stop:1 #ffffff);\
        border: 1px solid #a0a0a0;\
        border-top: 0px;\
        border-left: 0px;\
        border-right: 0px;\
    }\
    QHeaderView::section {\
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
            stop:0 #ffffff, stop:0.5 #e0e0e0, stop:1 #ffffff);\
        border: 1px solid #a0a0a0;\
        border-top: 0px;\
        border-left: 0px;\
        border-bottom: 0px;\
        padding-left: 4px; \
        padding-right: 4px; \
        padding-top: 1px; \
        padding-bottom: 2px; \
    }\
    QHeaderView::section:checked {\
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
            stop:0 #ffffff, stop:0.5 #a1c8ed, stop:1 #ffffff);\
    }\
    QHeaderView::section:pressed {\
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
            stop:0 #b0c6db, stop:0.5 #4588ca, stop:1 #b0c6db);\
    }";

const QString AppStyles::mac_vertHeaderStyle = "\
    QHeaderView::section {\
        background-color: #f0f0f0;\
        border: 1px solid #a0a0a0;\
        border-top: 0px;\
        border-left: 0px;\
        padding-left: 4px; \
        padding-right: 4px; \
    }\
    QHeaderView::section:checked {\
        background-color: #bfd5ea;\
    }\
    QHeaderView::section:pressed {\
        background-color: #5497d9;\
    }";

const QString AppStyles::mac_hSelectorButtonStyle = "QToolButton {\
    border: 1px solid #a0a0a0;\
    border-top: 0px;\
    border-left: 0px;\
    border-right: 0px;\
    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
        stop:0 #ffffff, stop:0.5 #e0e0e0, stop:1 #ffffff);\
}";

const QString AppStyles::mac_tableCornerStyle = "QTableCornerButton::section {\
    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
        stop:0 #ffffff, stop:0.5 #e0e0e0, stop:1 #ffffff);\
    border: 1px solid #a0a0a0;\
    border-top: 0px;\
    border-left: 0px;\
}";

const QString AppStyles::mac_tableWithBottomLineStyle = "QTableView {\
    border: 1px solid #a0a0a0;\
    border-top: 0px;\
    border-left: 0px;\
    border-right: 0px;\
}";
    
class AppStyles::Private {
public:
    static QStyle * simpleStyle;
};

QStyle * AppStyles::Private::simpleStyle = 0L;

QFont AppStyles::systemFont()
{
#ifdef Q_WS_MAC
    Str255 f_name;
    SInt16 f_size;
    Style f_style;    
    static const ScriptCode Script = smRoman;
    
    GetThemeFont(kThemeSmallSystemFont, Script, f_name, &f_size, &f_style);
    QFont font(qt_mac_from_pascal_string(f_name), f_size, (f_style & ::bold) ? QFont::Bold : QFont::Normal,
               (bool)(f_style & ::italic));
    
    return font;
#endif
    return qApp->font();
}

QStyle * AppStyles::simpleStyle()
{
#ifdef Q_WS_WIN
    return qApp->style();
#endif
    
    if ( !Private::simpleStyle )
        Private::simpleStyle = new QWindowsStyle;
    
    return Private::simpleStyle;
}

/*!
 Moves dialog to make it visible close to cell
 and not overlapping this cell. Dfeault position right to cell.
 If it can not fit then bottom to cell, and then another positions.
 */
void AppStyles::ensureVisibleNearRect(QRect globalRect, QWidget * dlg)
{
    QPoint globalPoint1 = globalRect.topLeft();
    QPoint globalPoint2 = globalRect.bottomRight();
    
    // draw the dialog off-screen to determine its size:
    int xmax = 0, ymax = 0;
    QDesktopWidget *desktop = QApplication::desktop();
    for ( int i = 0; i < desktop->numScreens(); ++i ) {
        xmax = qMax(xmax,desktop->screenGeometry().right());
        ymax = qMax(ymax,desktop->screenGeometry().bottom());
    }
    dlg->move(xmax+100,ymax+100);
    
    dlg->show();
    int dw = dlg->frameGeometry().width();
    int dh = dlg->frameGeometry().height();
    dlg->hide();
    
    QPoint pos = QRect(globalPoint1, globalPoint2).center();
    
    // try to show it right to shape
    if ( globalPoint2.x()+dw < xmax ) {
        pos.setX( globalPoint2.x()+5 );
        pos.setY( globalPoint1.y() );
        if (pos.y()+dh > ymax)
            pos.setY( globalPoint2.y()-dh );
    }
    
    // try to show left to shape
    else if ( globalPoint1.x()-dw > 0 ) {
        pos.setX( globalPoint1.x() -dw -5 );
        pos.setY( globalPoint1.y() );
        if (pos.y()+dh > ymax)
            pos.setY( globalPoint2.y()-dh );
    }
    
    // try to show under shape
    else if ( globalPoint2.y()+dh < ymax ) {
        pos.setX( globalPoint1.x() );
        pos.setY( globalPoint2.y()+5 );
        if ( pos.x()+dw > xmax)
            pos.setX( xmax-dw-5 );
        if ( pos.x() <0 )
            pos.setX(0);
    }
    
    // try to show above shape
    else if ( globalPoint1.y()-dh >0 ) {
        pos.setX( globalPoint1.x() );
        pos.setY( globalPoint1.y() -dh -5 );
        if ( pos.x()+dw > xmax)
            pos.setX( xmax-dw-5 );
        if ( pos.x() <0 )
            pos.setX(0);
    }
    
    // finally just put centered on shape
    else {
        int x = (globalPoint1.x()+globalPoint2.x())/2 - dw/2;
        int y = (globalPoint1.y()+globalPoint2.y())/2 - dh/2;
        pos.setX(x);
        pos.setY(y);
    }
    dlg->move( pos );
}

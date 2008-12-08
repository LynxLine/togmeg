
#ifndef APPSTYLES_H
#define APPSTYLES_H

class AppStyles {
public:

    //! Returns system application font
    static QFont systemFont();
    //! Returns simplified style for all platform (QWindowsStyle)
    static QStyle * simpleStyle();
    //! Make dialog visible near \a globalRect
    static void ensureVisibleNearRect(QRect globalRect, QWidget * dlg);

    static const QString win_toolbarStyle;
    static const QString mac_toolbarStyle;
    static const QString mac_horHeaderStyle;
    static const QString mac_vertHeaderStyle;
    static const QString mac_tableCornerStyle;
    static const QString mac_tableWithBottomLineStyle;
    static const QString mac_hSelectorButtonStyle;
    
private:
    class Private;
    Private * d;
    
};

#endif // APPSTYLES_H

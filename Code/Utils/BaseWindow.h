//
// Copyright Oleksandr Iakovliev, 2010
//

#ifndef BaseWindow_H
#define BaseWindow_H

#include <QMainWindow>

class BaseProject;

class QAction;
class QActionGroup;

class BaseWindow : public QMainWindow
{
    Q_OBJECT
public:
    BaseWindow(BaseProject * project, QWidget * parent = 0, Qt::WFlags flags = 0);
    virtual ~BaseWindow();
    
    BaseProject * project() const;
    
    QAction * action(QString) const;
    QActionGroup * actionGroup(QString) const;

    QMenu * fileMenu() const;
    QMenu * recentMenu() const;
    
    bool allowToClose();

public slots:
    virtual bool openFile(QString path ="");
    virtual bool saveFile(QString path ="");
    virtual bool saveAsFile();
    virtual bool revertToSaved();
    virtual void showWindowZoomed();

protected slots:
    virtual void readSettings();
    virtual void saveSettings() const;
    virtual void updateWindowTitle();
    virtual void updateFileMenu();
    virtual void updateRecentMenu();
    virtual void updateRecentList();
    virtual void clearRecentMenu();
    virtual bool openRecentFileActivated();

protected:
    void setAction(QString, QAction *);
    void setActionGroup(QString, QActionGroup *);
    
    virtual void createActions();
    virtual void closeEvent(QCloseEvent *);

private:
    class Private;
    Private * d;
};

#endif // BaseWindow_H

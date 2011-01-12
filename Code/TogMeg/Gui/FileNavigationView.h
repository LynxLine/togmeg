
#ifndef FileNavigationView_H
#define FileNavigationView_H

#include <QTreeView>
class LinkzProjectGui;
class FileNavigationModel;

class FileNavigationView : public QTreeView
{
    Q_OBJECT
public:        
    FileNavigationView(QWidget * parent = 0L);
    virtual ~FileNavigationView();

    void setModel(FileNavigationModel *);
    
signals:
    void widthChanged(int);
    void openFileRequest(const QString &);
    void filterLinksByFilePath(QString filePath);
    
protected:
    virtual void showEvent(QShowEvent * se);
    virtual void resizeEvent(QResizeEvent * re);
    virtual void keyReleaseEvent(QKeyEvent * ke);
    
private slots:
    void addItem();
    void removeItem();
    void selectItem(const QModelIndex &);
    void activateItem(const QModelIndex &);
    void m_aboutToLoad();
    void m_loaded();

private:
    class Private;
    Private * d;
};

#endif // FileNavigationView_H

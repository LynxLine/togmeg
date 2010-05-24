
#ifndef FILENAVIGATIONVIEW_H
#define FILENAVIGATIONVIEW_H

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
    virtual void leaveEvent(QEvent * e);
    virtual void showEvent(QShowEvent * se);
    virtual void resizeEvent(QResizeEvent * re);
    virtual void keyReleaseEvent(QKeyEvent * ke);
    virtual void mouseMoveEvent(QMouseEvent * me);
    virtual void mousePressEvent(QMouseEvent * me);
    
private slots:
    void activateItem(const QModelIndex &);

private:
    class Private;
    Private * d;
};

#include <QItemDelegate>

class FileNavigationViewDelegate : public QItemDelegate
{
public:
    FileNavigationViewDelegate(QObject * parent);
    virtual ~FileNavigationViewDelegate();
    
    QPoint mousePos;
    virtual QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
    virtual void paint(QPainter * p, const QStyleOptionViewItem & o, const QModelIndex & i) const;
    
private:
    void drawBox(QPainter * p, QRect r, QColor c) const;
    
    class Private;
    Private * d;
};

#endif // FILENAVIGATIONVIEW_H


#ifndef FILENAVIGATIONMODEL_H
#define FILENAVIGATIONMODEL_H

#include <QAbstractListModel>
class QFileInfo;

/*!
 \class FileNavigationModel
 */
class FileNavigationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Columns {
        ColName = 0,
        ColLinks,
        ColCount,
    };
    
    FileNavigationModel(QObject * parent);
    virtual ~FileNavigationModel();
    
    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    
    virtual QVariant data(const QModelIndex & i, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & i, const QVariant & v, int role);
        
    QFileInfo * fileInfo(const QModelIndex &) const;
    
    QString rootPath() const;
    QString path() const;
    
public slots:
    void reload();
    void setRootPath(const QString &);
    void loadPathContent(const QString &);
    
private:
    class Private;
    Private * d;
};

#endif // FILENAVIGATIONMODEL_H

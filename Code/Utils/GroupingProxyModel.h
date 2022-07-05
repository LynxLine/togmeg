#ifndef GROUPINGPROXYMODEL_H
#define GROUPINGPROXYMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QAbstractProxyModel>

class GroupingProxyModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    GroupingProxyModel(QObject * parent = 0);
    virtual ~GroupingProxyModel();

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;

    QModelIndex mapFromSource(const QModelIndex & sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex & proxyIndex) const;

//    virtual QItemSelection mapSelectionToSource(const QItemSelection &selection) const;
//    virtual QItemSelection mapSelectionFromSource(const QItemSelection &selection) const;

    virtual void setSourceModel(QAbstractListModel * listModel);
    virtual void setGrouping(int keyColumn, int keyShowColumn, int valueColumn, int firstColumnAt = 0);

    virtual void setShowEmptyRows(bool);
    virtual bool showEmptryRows() const;

    virtual QModelIndexList sourceTreeIndexes(QModelIndex & parent);

    virtual QAbstractListModel * source_model() const;

signals:
    void doubleClicked(const QModelIndex &);
    void expandTree(QModelIndex);

protected:
    virtual void addRows(QModelIndex, int, int);
    virtual void removeRows(QModelIndex, int, int);
    virtual void clear();

    virtual void connectToModel();
    virtual void disconnectFromModel();

    QStringList mimeTypes() const;
    virtual QMimeData * mimeData(const QModelIndexList & indexes ) const;
    virtual bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
    virtual Qt::DropActions supportedDropActions() const;

public slots:
    virtual void initModel();

private slots:
//    virtual void source_dataChanged(QModelIndex, QModelIndex);
//    virtual void source_headerDataChanged(Qt::Orientation, int, int);
//    virtual void source_rowsAboutToBeInserted(QModelIndex, int, int);
    virtual void source_rowsInserted(QModelIndex, int, int);
//    virtual void source_columnsAboutToBeInserted(QModelIndex, int, int);
//    virtual void source_columnsInserted(QModelIndex, int, int);
    virtual void source_rowsAboutToBeRemoved(QModelIndex, int, int);
    virtual void source_rowsRemoved(QModelIndex, int, int);
//    virtual void source_columnsAboutToBeRemoved(QModelIndex, int, int);
//    virtual void source_columnsRemoved(QModelIndex, int, int);
//    virtual void source_layoutAboutToBeChanged();
//    virtual void source_layoutChanged();
//    virtual void source_modelReset();
    virtual void proxy_doubleClicked(const QModelIndex&);
    virtual void source_dataChanged(QModelIndex, QModelIndex);
    void source_modelReset();

private:
    class Private;
    Private * d;
    class Container;
    Container * container;

};

#endif // GROUPINGPROXYMODEL_H

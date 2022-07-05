//
// Copyright Petrostreamz 2008
//

#include <QtCore>
#include "GroupingProxyModel.h"

// Container internal class implementation
class GroupingProxyModel::Container
{
public:
    Container(Container * parent = 0)
    {
        parentItem = parent;
    }
    Container(QVariant & data, QVariant & keyData, QVariant & decoData, Container * parent = 0)
    {
        parentItem = parent;
        itemData = data;
        itemKeyData = keyData;
        itemDecoData = decoData;
    }
    virtual ~Container() { qDeleteAll(childItems); }

    void appendChild(Container * child)
    {
        childItems.append(child);
    }

    void insertChild(int row, Container * child)
    {
        childItems.insert(row, child);
    }

    void removeChild(int row)
    {
        delete childItems.takeAt(row);
    }

    Container * child(int row)
    {
        return childItems.value(row);
    }

    int childCount() const
    {
        return childItems.count();
    }

    int row() const
    {
        if (parentItem)
            return parentItem->childItems.indexOf(const_cast<Container*>(this));

        return 0;
    }

    Container * parent()
    {
        return parentItem;
    }

    QList<Container*> & children()
    {
        return childItems;
    }

    QVariant data() const
    {
        return itemData;
    }

    QVariant keyData() const
    {
        return itemKeyData;
    }

    QVariant decoData() const
    {
        return itemDecoData;
    }
    
    void setData(QVariant newData, QVariant newKeyData)
    {
        itemData = newData;
        itemKeyData = newKeyData;
    }

private:
    QList<Container*> childItems;
    QVariant itemData;
    QVariant itemKeyData;
    QVariant itemDecoData;
    Container * parentItem;
};

class GroupingProxyModel::Private
{
public:
    QAbstractListModel * listModel;
    int keyColumn;
    int keyShowColumn;
    int valueColumn;
    int source_columnCount;
    int firstColumnAt;
    bool showEmptyRows;
    QVariantList header;
    QHash<QPersistentModelIndex, QPersistentModelIndex> mapFromSource;
    QHash<QPersistentModelIndex, QPersistentModelIndex> mapToSource;
    QMap<QString, int> savedKeys;
};

// GroupingProxyModel
GroupingProxyModel::GroupingProxyModel(QObject * parent)
: QAbstractItemModel(parent)
{
    d = new Private;
    d->listModel = 0L;
    container = 0L;
    d->keyColumn = -1;
    d->keyShowColumn = -1;
    d->valueColumn = -1;
    d->source_columnCount = 0;
    d->firstColumnAt = 0;
    d->showEmptyRows = true;
}

GroupingProxyModel::~GroupingProxyModel()
{
    delete container;
    delete d;
}

Qt::ItemFlags GroupingProxyModel::flags(const QModelIndex & index) const
{
    if (!index.isValid())
        return 0;

    if (!index.parent().isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;

    return d->listModel->flags(mapToSource(index));
}

QVariant GroupingProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    /* fetches header data text into d->header from start
       (TODO: find root of the problem with direct headerData() linking to the source model
    */
    if (d->header.isEmpty()) {
        if (d->source_columnCount > 0) {
            for (int col=0;col<d->source_columnCount;col++) {
                d->header.append(d->listModel->headerData(col, Qt::Horizontal, Qt::DisplayRole));
            }
        }
    }

    if (role == Qt::DisplayRole) {
        return d->header.at(section);
    }

    return QVariant();
}

int GroupingProxyModel::rowCount(const QModelIndex & parent) const
{
    Container * parentItem;
//    if (parent.column() > 0)
//        return 0;

    if (!parent.isValid())
        parentItem = container;
    else
        parentItem = static_cast<Container*>(parent.internalPointer());

    return parentItem->childCount();
}

int GroupingProxyModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->source_columnCount;
}

QModelIndex GroupingProxyModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Container * parentItem;

    if (!parent.isValid())
        parentItem = container;
    else
        parentItem = static_cast<Container*>(parent.internalPointer());

    Container * childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex GroupingProxyModel::parent(const QModelIndex & index) const
{
    if (!index.isValid())
        return QModelIndex();

    Container * childItem = static_cast<Container*>(index.internalPointer());
    Container * parentItem = childItem->parent();

    if (parentItem == container)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

QModelIndex GroupingProxyModel::mapFromSource(const QModelIndex & sourceIndex) const
{
    return d->mapFromSource.value(sourceIndex);
}

QModelIndex GroupingProxyModel::mapToSource(const QModelIndex & proxyIndex) const
{
    return d->mapToSource.value(proxyIndex);
}

//QItemSelection GroupingProxyModel::mapSelectionToSource(const QItemSelection & proxySelection) const
//{
//    qDebug() << "called s";
//    QModelIndexList proxyIndexes = proxySelection.indexes();
//    QItemSelection sourceSelection;
//    for (int i = 0; i < proxyIndexes.size(); ++i)
//        sourceSelection << QItemSelectionRange(mapToSource(proxyIndexes.at(i)));
//    return sourceSelection;
//}
//
//QItemSelection GroupingProxyModel::mapSelectionFromSource(const QItemSelection & sourceSelection) const
//{
//    qDebug() << "called p";
//    QModelIndexList sourceIndexes = sourceSelection.indexes();
//    QItemSelection proxySelection;
//    for (int i = 0; i < sourceIndexes.size(); ++i)
//        proxySelection << QItemSelectionRange(mapFromSource(sourceIndexes.at(i)));
//    return proxySelection;
//}

QVariant GroupingProxyModel::data(const QModelIndex & i, int role) const
{
    if (!i.isValid())
        return QVariant();
    
    if (i.parent().isValid()) {
        if (mapToSource(i).isValid())
            return mapToSource(i).data(role);
    } 
    else if (!i.parent().isValid() && i.column() == d->firstColumnAt) {
        if (role == Qt::DisplayRole)
            return static_cast<Container*>(i.internalPointer())->keyData();
        if (role == Qt::DecorationRole)
            return static_cast<Container*>(i.internalPointer())->decoData();
    }

    return QVariant();
}

bool GroupingProxyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    Q_UNUSED(value);
    if ( !index.isValid() )
        return false;

    if (d->listModel->setData(mapToSource(index), value, role)) {
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void GroupingProxyModel::setShowEmptyRows(bool value)
{
    d->showEmptyRows = value;
}

bool GroupingProxyModel::showEmptryRows() const
{
    return d->showEmptyRows;
}

void GroupingProxyModel::setSourceModel(QAbstractListModel * listModel)
{
    disconnectFromModel();
    d->header.clear();
    d->savedKeys.clear();
    d->mapFromSource.clear();
    d->mapToSource.clear();
    d->listModel = listModel;
    delete container;
    int source_colCount = 0;
    while (!listModel->headerData(source_colCount, Qt::Horizontal, Qt::DisplayRole).isNull()) {
        ++source_colCount;
    }
    d->source_columnCount = source_colCount;
    container = new Container();
    connectToModel();
}

void GroupingProxyModel::setGrouping(int keyColumn, int keyShowColumn, int valueColumn, int firstColumnAt)
{
    d->keyColumn = keyColumn;
    d->keyShowColumn = keyShowColumn;
    d->valueColumn = valueColumn;
    d->firstColumnAt = firstColumnAt;
    initModel(); // in case if source model contains some data
}

QAbstractListModel * GroupingProxyModel::source_model() const
{
    return d->listModel;
}

void GroupingProxyModel::initModel()
{
    for (int row=0;row<d->listModel->rowCount();row++) {
        addRows(QModelIndex(), row, row);
    }
}

void GroupingProxyModel::addRows(QModelIndex parent, int from, int to)
{
    Q_UNUSED(parent);
    for (int row=from;row<=to;row++) {
        QModelIndex source_key_index = d->listModel->index(row, d->keyColumn, QModelIndex());
        QModelIndex source_keyshow_index = d->listModel->index(row, d->keyShowColumn, QModelIndex());
        QVariant source_key_data = source_key_index.data(Qt::DisplayRole);
        QVariant source_keyshow_data = source_keyshow_index.data(Qt::DisplayRole);
        QVariant source_keyshow_deco = source_keyshow_index.data(Qt::DecorationRole);
        bool key_exists = false;
        int key_row = -1;
        for (int i=0;i<container->childCount();i++) {
            if (container->child(i)->data().toString() == source_key_data.toString()) {
                key_exists = true;
                key_row = i;
                break;
            }
        }

        QPersistentModelIndex parentIndex;
        if (!key_exists) {
            key_row = container->childCount();
            if (d->savedKeys.value(source_key_data.toString(), -1) >= 0)
                key_row = d->savedKeys.value(source_key_data.toString());
            beginInsertRows(QModelIndex(), key_row, key_row);
            container->insertChild(key_row, new Container(source_key_data, 
                                                          source_keyshow_data, 
                                                          source_keyshow_deco, 
                                                          container));
            parentIndex = index(key_row, d->firstColumnAt, QModelIndex());
            endInsertRows();
            if (d->showEmptyRows) {
                d->mapFromSource[parentIndex] = parentIndex;
                d->mapToSource[parentIndex] = parentIndex;
            } else {
                QPersistentModelIndex source_index = d->listModel->index(row, d->valueColumn, QModelIndex());
                if (source_index.isValid() && source_index.data(Qt::DisplayRole).isNull()) {
                    d->mapToSource[parentIndex] = source_index;
                    d->mapFromSource[source_index] = parentIndex;
                    return;
                } else {
                    d->mapFromSource[parentIndex] = parentIndex;
                    d->mapToSource[parentIndex] = parentIndex;
                }
            }
        } else {
            parentIndex = index(key_row, d->firstColumnAt, QModelIndex());
        }

        if (key_row < container->childCount()) {
            beginInsertRows(parentIndex, rowCount(parentIndex), rowCount(parentIndex));
            container->child(key_row)->appendChild(new Container(container->child(key_row)));
            for (int col=0;col<columnCount();col++) {
                QPersistentModelIndex source_index = d->listModel->index(row, col, QModelIndex());
                if (col == d->firstColumnAt) {
                    source_index = d->listModel->index(row, d->valueColumn, QModelIndex());
                }
                QPersistentModelIndex proxy_index = index(rowCount(parentIndex)-1, col, parentIndex);
                d->mapFromSource[source_index] = proxy_index;
                d->mapToSource[proxy_index] = source_index;
            }
            endInsertRows();
        }
    }

//    qDebug() << "from ---";
//    foreach (QModelIndex i, d->mapFromSource.keys())
//        qDebug() << "key: " << i << "value: " << d->mapFromSource.value(i);
//    qDebug() << "\n--\n";
//    qDebug() << "to ---";
//    foreach (QModelIndex i, d->mapToSource.keys())
//        qDebug() << "key: " << i << "value: " << d->mapToSource.value(i);
}

void GroupingProxyModel::removeRows(QModelIndex parent, int from, int to)
{
    Q_UNUSED(parent);
    for (int row=from;row<=to;row++) {
        QModelIndex source_row_index = d->listModel->index(row, d->valueColumn, QModelIndex());
        QModelIndex row_index = d->mapFromSource.value(source_row_index);
        QModelIndex key_index = row_index.parent();
        if (row_index.isValid() && !row_index.parent().isValid()) { // parent tree row
//            qDebug() << key_index << row_index << source_row_index;
//            bool all_empty_childs = true;
//            for (int i=0;i<rowCount(row_index);i++) {
//                if (!row_index.child(i, d->valueColumn).data(Qt::DisplayRole).isNull())
//                    all_empty_childs = false;
//            }
//            if (all_empty_childs) {
                d->mapFromSource.remove(source_row_index);
                d->mapToSource.remove(row_index);
                beginRemoveRows(QModelIndex(), row_index.row(), row_index.row());
                d->savedKeys[container->child(row_index.row())->data().toString()] = row_index.row();
                container->removeChild(row_index.row());
                endRemoveRows();
//            }
        } else if (row_index.isValid() && key_index.isValid()) { // child row
            for (int col=0;col<columnCount();col++) {
                QModelIndex col_row_index = index(row_index.row(), col, key_index);
                d->mapFromSource.remove(d->mapToSource.take(col_row_index));
            }
            beginRemoveRows(key_index, row_index.row(), row_index.row());
            container->child(key_index.row())->removeChild(row_index.row());
            endRemoveRows();
            if (container->child(key_index.row())->childCount() == 0) {
                if (d->mapToSource.value(key_index) == key_index) {
                    d->mapFromSource.remove(d->mapToSource.take(key_index));
                    beginRemoveRows(QModelIndex(), key_index.row(), key_index.row());
                    d->savedKeys[container->child(key_index.row())->data().toString()] = key_index.row();
                    container->removeChild(key_index.row());
                    endRemoveRows();
                }
            }
        }
    }

//    qDebug() << "left: ";
//    qDebug() << d->mapFromSource;
//    qDebug() << "\n--\n";
//    qDebug() << d->mapToSource;

}

QModelIndexList GroupingProxyModel::sourceTreeIndexes(QModelIndex & parent)
{
    QModelIndexList sourceIndexes;
    if (!parent.isValid() && rowCount(parent) == 0)
        return sourceIndexes;

    for (int row=rowCount(parent)-1;row>=0;row--) {
          sourceIndexes.append(mapToSource(parent.child(row, d->firstColumnAt)));
    }
    return sourceIndexes;
}

void GroupingProxyModel::clear()
{
    d->savedKeys.clear();
    d->mapFromSource.clear();
    d->mapToSource.clear();
    if (rowCount()-1 < 0)
        return;
    beginRemoveRows(QModelIndex(), 0, rowCount()-1);
    for (int i =container->childCount()-1; i>=0; i--)
        delete container->children().takeAt(i);
    endRemoveRows();
}

// signals/slots connection
void GroupingProxyModel::source_rowsInserted(QModelIndex parent, int from, int to)
{
    addRows(parent, from, to);
    if (rowCount() == 1) {
        emit expandTree(index(0,0, QModelIndex()));
    }
}

void GroupingProxyModel::source_rowsAboutToBeRemoved(QModelIndex parent, int from, int to)
{
    removeRows(parent, from, to);
}

void GroupingProxyModel::source_rowsRemoved(QModelIndex parent, int from, int to)
{
    Q_UNUSED(parent);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void GroupingProxyModel::proxy_doubleClicked(const QModelIndex & proxyIndex) {
    if (proxyIndex.parent().isValid()) {
        emit doubleClicked(mapToSource(proxyIndex));
    }
}

void GroupingProxyModel::source_dataChanged(QModelIndex source_start_index, QModelIndex source_end_index)
{
    QModelIndex source_index = d->listModel->index(source_start_index.row(), d->valueColumn, QModelIndex());
    emit dataChanged(mapFromSource(source_start_index), mapFromSource(source_end_index));
    QModelIndex current_index = mapFromSource(source_index);
    Container * parent_key = 0;
    if (current_index.parent().isValid()) { // update the parent key
        parent_key = static_cast<Container*>(current_index.parent().internalPointer());
    } else if (!d->showEmptyRows) {
        parent_key = static_cast<Container*>(mapFromSource(source_index).internalPointer());
    }
    if (parent_key) {
        if (parent_key->data() != d->listModel->index(source_index.row(), d->keyColumn, QModelIndex()).data(Qt::DisplayRole) ||
            parent_key->keyData() != d->listModel->index(source_index.row(), d->keyShowColumn, QModelIndex()).data(Qt::DisplayRole)) {
            parent_key->setData(d->listModel->index(source_index.row(), d->keyColumn, QModelIndex()).data(Qt::DisplayRole),
                                d->listModel->index(source_index.row(), d->keyShowColumn, QModelIndex()).data(Qt::DisplayRole)
                                );
            emit dataChanged(current_index.parent(), current_index.parent());
        }
    }
    // duplicate parents relocation
    QHash<QString, int> keys;
    for (int idx=container->childCount()-1;idx>=0;idx--) {
        QString keyData = container->child(idx)->keyData().toString();
        if (keys.contains(keyData)) {
            QList<QModelIndex> sourceDataIndexes;
            for (int row=0;row<container->child(keys[keyData])->childCount();row++) {
                QModelIndex sourceParentIndex = index(keys[keyData], d->firstColumnAt, QModelIndex());
                QModelIndex sourceDataIndex = mapToSource(index(row, 0, sourceParentIndex));
                sourceDataIndexes.append(sourceDataIndex);
            }
            for (int row=0;row<sourceDataIndexes.count();row++) {
                QModelIndex sourceDataIndex = sourceDataIndexes.at(row);
                removeRows(QModelIndex(), sourceDataIndex.row(), sourceDataIndex.row());
                addRows(QModelIndex(), sourceDataIndex.row(), sourceDataIndex.row());
            }
        }
        keys[keyData] = idx;
    }
}

void GroupingProxyModel::connectToModel()
{
    connect(d->listModel, SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(source_rowsInserted(QModelIndex, int, int)));
    connect(d->listModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), this, SLOT(source_rowsAboutToBeRemoved(QModelIndex, int, int)));
    connect(d->listModel, SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(source_rowsRemoved(QModelIndex, int, int)));
    connect(d->listModel, SIGNAL(layoutAboutToBeChanged()), this, SIGNAL(layoutAboutToBeChanged()));
    connect(d->listModel, SIGNAL(layoutChanged()), this, SIGNAL(layoutChanged()));
    connect(d->listModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(source_dataChanged(QModelIndex,QModelIndex)));
    connect(d->listModel, SIGNAL(modelReset()), this, SLOT(source_modelReset()));
}

void GroupingProxyModel::disconnectFromModel()
{
    if (d->listModel) {
        disconnect(SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(source_rowsInserted(QModelIndex,int,int)));
        disconnect(SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), this, SLOT(source_rowsAboutToBeRemoved(QModelIndex, int, int)));
        disconnect(SIGNAL(rowsRemoved(QModelIndex, int, int)), this, SLOT(source_rowsRemoved(QModelIndex, int, int)));
        disconnect(SIGNAL(layoutAboutToBeChanged()), this, SIGNAL(layoutAboutToBeChanged()));
        disconnect(SIGNAL(layoutChanged()), this, SIGNAL(layoutChanged()));
        disconnect(SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(source_dataChanged(QModelIndex,QModelIndex)));
        disconnect(SIGNAL(modelReset()), this, SLOT(source_modelReset()));
    }
}

void GroupingProxyModel::source_modelReset()
{
    clear();
    initModel();
    emit expandTree(index(0,0, QModelIndex()));
}

QStringList GroupingProxyModel::mimeTypes() const
{
    return d->listModel->mimeTypes();
}

QMimeData * GroupingProxyModel::mimeData ( const QModelIndexList & indexes ) const
{
    QModelIndexList convertedIndexes;
    for (int i=0;i<indexes.count();i++) {
        convertedIndexes.append(mapToSource(indexes.at(i)));
    }
    return d->listModel->mimeData(convertedIndexes);
}

bool GroupingProxyModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    if (parent.parent().isValid()) {
        return d->listModel->dropMimeData(data, action, row, column, mapToSource(parent));
    } else if (row == -1 && column == -1) {
        return d->listModel->dropMimeData(data, action, row, column, mapToSource(parent));
    } else {
        return false;
    }
}

Qt::DropActions GroupingProxyModel::supportedDropActions() const
{
    return d->listModel->supportedDropActions();
}

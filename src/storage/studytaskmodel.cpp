//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "crammero.h"
#include "datacontainer.h"
#include "studytaskmodel.h"

class StudyTaskModel::Private
{
public:
    QList<QString> entries;
};

/*!
 Crestes StudyTaskModel
 */
StudyTaskModel::StudyTaskModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;


}

/*!
 Deletes the object.
 */
StudyTaskModel::~StudyTaskModel()
{
    qDebug() << "~StudyTaskModel";
    delete d;
}

int StudyTaskModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->entries.count();
}

QVariant StudyTaskModel::data(const QModelIndex & index, int role) const
{
    /*
    StudyTask * studyTask = task(index);
    if ( !studyTask ) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return studyTask->name();
    }
    */

    return QVariant();
}

bool StudyTaskModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    /*
    StudyTask * studyTask = task(index);
    if ( !studyTask ) return false;
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        studyTask->setName( value.toString() );
        return true;
    }
    */

    return false;
}

QVariant StudyTaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    if (role == Qt::DisplayRole) {
        return tr("Question:");
    }
    return QVariant();
}

Qt::ItemFlags StudyTaskModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

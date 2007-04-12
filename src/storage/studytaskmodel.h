//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef STUDYTASKMODEL_H
#define STUDYTASKMODEL_H

#include <QAbstractListModel>

class StudyTaskModel : public QAbstractListModel
{
Q_OBJECT

public:
    StudyTaskModel(QObject * parent = 0L);
    virtual ~StudyTaskModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	class Private;
	Private * d;

};

#endif // STUDYTASKMODEL_H

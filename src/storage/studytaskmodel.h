//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef STUDYTASKMODEL_H
#define STUDYTASKMODEL_H

#include <QAbstractListModel>

class StudyDataEntry 
{
public:
    QString question;
    QString answer;
};

class StudyTaskModel : public QAbstractListModel
{
Q_OBJECT

public:
    StudyTaskModel(QObject * parent = 0L);
    virtual ~StudyTaskModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QModelIndex addNewEntry();
    void removeEntry(QModelIndex index);

public slots:
    void load(QString taskId);
    void save();

private:
	class Private;
	Private * d;

};

#endif // STUDYTASKMODEL_H

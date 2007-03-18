//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CATALOGMODEL_H
#define CATALOGMODEL_H

#include <QAbstractItemModel>

/*!
 * \class CatalogItem
 */
class CatalogItem 
{
friend class CatalogModel;
public:
    CatalogItem(QAbstractItemModel * model);
    CatalogItem(CatalogItem * parent);
    virtual ~CatalogItem();

    QString text();
    void setText(QString);

    QAbstractItemModel * model();

    int row();
    CatalogItem * parent();
    CatalogItem * child(int index);
    QList<CatalogItem *> & children();
    void remove(int index);
    int count();
    int level();

protected:
    //! implemented for providing data for Model.
    virtual QVariant data(int column, int role);

private:
    void append(CatalogItem * child);

private:
    QList<CatalogItem *> _children;
    CatalogItem * _parent;

    QAbstractItemModel * _model;
    QString _text;
    int _level;
};

/*!
 */
class CatalogModel : public QAbstractItemModel
{
Q_OBJECT

public:
    CatalogModel(QObject * parent = 0L);
    virtual ~CatalogModel();

    virtual CatalogItem * root() const;

    QModelIndex parent(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;

private:
	class Private;
	Private * d;

};

QDomDocument& operator>>(QDomDocument & doc, CatalogModel * catalogModel);
QDomDocument& operator<<(QDomDocument & doc, CatalogModel * catalogModel);
QDomNode& operator>>(QDomNode & node, CatalogItem * item);
QDomNode& operator<<(QDomNode & node, CatalogItem * item);

#endif // CATALOGMODEL_H

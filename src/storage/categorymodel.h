//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CategoryModel_H
#define CategoryModel_H

#include <QAbstractItemModel>

/*!
 * \class CategoryItem
 */
class CategoryItem 
{
friend class CategoryModel;
public:
    CategoryItem(QAbstractItemModel * model);
    CategoryItem(QString id, int index, CategoryItem * parent);
    CategoryItem(QString id, CategoryItem * parent);
    virtual ~CategoryItem();

    QString id();
    QString compositeId();

    QString text();
    void setText(QString);

    bool isExpanded();
    void setExpanded(bool);

    QAbstractItemModel * model();

    int row();
    CategoryItem * parent();
    CategoryItem * child(int index);
    QList<CategoryItem *> & children();
    void remove(int index);
    int count();
    int level();

protected:
    //! implemented for providing data for Model.
    virtual QVariant data(int role);
    bool setData(QVariant value, int role);

private:
    void append(CategoryItem * child);
    void insert(int index, CategoryItem * child);

private:
    QList<CategoryItem *> _children;
    CategoryItem * _parent;

    QAbstractItemModel * _model;
    QString _text, _id;
    bool _expanded;
    int _level;
};

/*!
 */
class CategoryModel : public QAbstractItemModel
{
Q_OBJECT

public:
    CategoryModel(QObject * parent = 0L);
    virtual ~CategoryModel();

    virtual CategoryItem * root() const;
    CategoryItem * item(const QModelIndex &index);

    QModelIndex parent(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex indexOf(CategoryItem * item);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    CategoryItem * createItem(QString name, CategoryItem * parent);
    void removeItem(CategoryItem *);
    void updateItem(CategoryItem *);

private:
	class Private;
	Private * d;

};

QDomDocument& operator>>(QDomDocument & doc, CategoryModel * categoryModel);
QDomDocument& operator<<(QDomDocument & doc, CategoryModel * categoryModel);
QDomNode& operator>>(QDomNode & node, CategoryItem * item);
QDomNode& operator<<(QDomNode & node, CategoryItem * item);

#endif // CategoryModel_H

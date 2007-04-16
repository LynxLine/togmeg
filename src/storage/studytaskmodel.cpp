//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "crammero.h"
#include "studytask.h"
#include "datacontainer.h"
#include "tasklistmodel.h"
#include "studytaskmodel.h"

class StudyTaskModel::Private
{
public:
    QList<StudyDataEntry> entries;
    DataContainer * dataContainer;
};

/*!
 Crestes StudyTaskModel
 */
StudyTaskModel::StudyTaskModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
    d->dataContainer = 0L;
}

/*!
 Deletes the object.
 */
StudyTaskModel::~StudyTaskModel()
{
    qDebug() << "~StudyTaskModel";
    if ( d->dataContainer ) save();
    delete d;
}

void StudyTaskModel::load(QString taskId)
{
    qDebug() << "StudyTaskModel::load()," << taskId;

    if ( d->dataContainer ) save();
    
    d->dataContainer = 0L;
    d->entries.clear();

    StudyTask * task = TaskListModel::instance()->task( taskId );
    if ( !task ) return;

    d->dataContainer = task->dataContainer();
        
    QIODevice * resource = d->dataContainer->resource("content.xml");
    if (resource) {
        QDomDocument doc;
        doc.setContent( resource );
        delete resource;

        QDomElement el = doc.documentElement();
        QDomNode n = el.firstChild();
        while (!n.isNull()) {
            QDomElement questionEl = n.firstChildElement("question");
            QDomElement answerEl   = n.firstChildElement("answer");

            StudyDataEntry entry;
            entry.question = questionEl.firstChild().toText().data();
            entry.answer   = answerEl.firstChild().toText().data();

            d->entries << entry;

            n = n.nextSibling();
        }
    }

    qDebug() << d->entries.count()<< " entries loaded";

    reset();
}

void StudyTaskModel::save()
{
    if ( !d->dataContainer ) return;
        
    QIODevice * resource = d->dataContainer->create("content.xml");
    if (resource) {
        QDomDocument doc("taskcontentxml");

        QDomElement docEl = doc.createElement("content");
        doc.appendChild(docEl);

        foreach (StudyDataEntry entry, d->entries) {
            QDomElement entryEl = doc.createElement("studyentry");
            docEl.appendChild(entryEl);

            QDomElement questionEl = doc.createElement("question");
            entryEl.appendChild(questionEl);

            QDomText questionTextEl = doc.createTextNode( entry.question );
            questionEl.appendChild(questionTextEl);

            QDomElement answerEl = doc.createElement("answer");
            entryEl.appendChild(answerEl);

            QDomText answerTextEl = doc.createTextNode( entry.answer );
            answerEl.appendChild(answerTextEl);
        }

        resource->write(doc.toByteArray());

        delete resource;
    }
}

int StudyTaskModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->entries.count();
}

int StudyTaskModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant StudyTaskModel::data(const QModelIndex & index, int role) const
{
    if ( !index.isValid() ) return QVariant();
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( index.column() == 0 ) return d->entries[ index.row() ].question;
        if ( index.column() == 1 ) return d->entries[ index.row() ].answer;
    }

    return QVariant();
}

bool StudyTaskModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if ( !index.isValid() ) return false;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return false;

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( index.column() == 0 ) d->entries[ index.row() ].question = value.toString();
        if ( index.column() == 1 ) d->entries[ index.row() ].answer   = value.toString();
        return true;
    }

    return false;
}

QVariant StudyTaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    if (role == Qt::DisplayRole) {
        if ( section == 0)
            return tr("Question");
        else if ( section == 1 )
            return tr("Answer");
    }
    return QVariant();
}

Qt::ItemFlags StudyTaskModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

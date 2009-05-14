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
    static StudyTaskModel * instance;
    QList<StudyDataEntry> entries;
    DataContainer * dataContainer;
    QString taskId;
};

StudyTaskModel * StudyTaskModel::Private::instance = 0L;

/*!
 Crestes StudyTaskModel
 */
StudyTaskModel::StudyTaskModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
    Private::instance = this;
    d->dataContainer = 0L;
}

StudyTaskModel * StudyTaskModel::instance()
{
    return Private::instance;
}

/*!
 Deletes the object.
 */
StudyTaskModel::~StudyTaskModel()
{
    qDebug() << "~StudyTaskModel";
    delete d;
}

QString StudyTaskModel::taskId()
{
    return d->taskId;
}

void StudyTaskModel::loadFile(QString filePath)
{
    qDebug() << "StudyTaskModel::loadFile()," << filePath;
    
    d->dataContainer = 0L;
    d->entries.clear();
    d->taskId.clear();
    
    disconnect(this, SIGNAL(rowCountChanged(int)),0,0);
    
    
    QIODevice * resource = new QFile(filePath);
    if (resource->open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        doc.setContent( resource );
        
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
    delete resource;
    
    emit rowCountChanged( rowCount() );
    reset();
}

void StudyTaskModel::saveFile(QString filePath)
{
    QIODevice * resource = new QFile(filePath);
    if (resource->open(QIODevice::WriteOnly)) {
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
        
    }
    delete resource;
}

QModelIndex StudyTaskModel::addNewEntry()
{
    beginInsertRows(QModelIndex(), d->entries.count(), d->entries.count());
    StudyDataEntry entry;
    d->entries << entry;
    endInsertRows();

    emit rowCountChanged( rowCount() );
    return index( d->entries.count()-1,1 );
}

void StudyTaskModel::removeEntry(QModelIndex index)
{
    if ( !index.isValid() ) return;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return;

    int i = index.row();
    beginRemoveRows(QModelIndex(), i, i);
    d->entries.removeAt(i);
    endRemoveRows();

    emit rowCountChanged( rowCount() );
}

int StudyTaskModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->entries.count();
}

int StudyTaskModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant StudyTaskModel::data(const QModelIndex & index, int role) const
{
    if ( !index.isValid() ) return QVariant();
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( index.column() == IdColumn       ) return QString("  %1 ").arg(index.row()+1);
        if ( index.column() == QuestionColumn ) return d->entries[ index.row() ].question;
        if ( index.column() == AnswerColumn   ) return d->entries[ index.row() ].answer;
    }

    if (role == Qt::TextAlignmentRole && index.column() == IdColumn) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return QVariant();
}

bool StudyTaskModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if ( !index.isValid() ) return false;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return false;

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( index.column() == QuestionColumn ) d->entries[ index.row() ].question = value.toString();
        if ( index.column() == AnswerColumn   ) d->entries[ index.row() ].answer   = value.toString();

        //analize last row
        QModelIndex lastQ = this->index( rowCount()-1, QuestionColumn);
        QModelIndex lastA = this->index( rowCount()-1, AnswerColumn);
        if (!data(lastQ, Qt::DisplayRole).toString().isEmpty() ||
            !data(lastA, Qt::DisplayRole).toString().isEmpty()) {
            addNewEntry();
        }

        return true;
    }

    return false;
}

QVariant StudyTaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole) {
        if ( section == IdColumn)
            return tr("#");
        else if ( section == QuestionColumn)
            return tr("Question");
        else if ( section == AnswerColumn )
            return tr("Answer");
    }

    if (role == Qt::TextAlignmentRole && section == IdColumn) {
        return Qt::AlignRight;
    }

    return QVariant();
}

Qt::ItemFlags StudyTaskModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return Qt::ItemIsDropEnabled;

    if ( index.column() == IdColumn ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

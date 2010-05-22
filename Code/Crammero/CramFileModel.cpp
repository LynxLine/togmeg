//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtXml>
#include <QtCore>

#include "crammero.h"
#include "CramFileModel.h"

class CramFileModel::Private
{
public:
    static CramFileModel * instance;
    QList<StudyDataEntry> entries;
    bool isModified;
    QString filePath;
};

/*!
 Crestes CramFileModel
 */
CramFileModel::CramFileModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
    d->isModified = false;
    addNewEntry();
}

/*!
 Deletes the object.
 */
CramFileModel::~CramFileModel()
{
    delete d;
}

void CramFileModel::loadTabFile(QString filePath)
{
    qDebug() << "CramFileModel::loadTabFile()," << filePath;
    
    d->entries.clear();
    d->filePath = filePath;
    
    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly)) {
        while (!f.atEnd()) {
            QString line = QString::fromUtf8(f.readLine());
            QStringList args = line.split("\t");
            if (args.count() <2) continue;
            
            StudyDataEntry entry;
            entry.question = args[0].simplified();
            entry.answer   = args[1].simplified();
            d->entries << entry;
        }
    }
    
    reset();
    addNewEntry();
}

void CramFileModel::loadXmlFile(QString filePath)
{
    qDebug() << "CramFileModel::loadXmlFile()," << filePath;
    
    d->entries.clear();
    d->filePath = filePath;
    
    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        doc.setContent(&f);
        
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
    
    reset();
    addNewEntry();
}

bool CramFileModel::saveTabFile(QString filePath)
{
    QFile f(filePath);
    if (f.open(QIODevice::WriteOnly)) {
        foreach (StudyDataEntry entry, d->entries) {
            if (entry.question.isEmpty() && entry.answer.isEmpty())
                continue;
            
            QString line = entry.question+"\t"+entry.answer+"\n";
            f.write(line.toUtf8());
        }
        
        d->filePath = filePath;
        return true;
    }
    
    return false;
}

bool CramFileModel::saveXmlFile(QString filePath)
{
    QFile f(filePath);
    if (f.open(QIODevice::WriteOnly)) {
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
        
        f.write(doc.toByteArray());

        d->filePath = filePath;
        return true;
    }
    
    return false;
}

QModelIndex CramFileModel::addNewEntry()
{
    beginInsertRows(QModelIndex(), d->entries.count(), d->entries.count());
    StudyDataEntry entry;
    d->entries << entry;
    endInsertRows();

    return index( d->entries.count()-1,1 );
}

void CramFileModel::removeEntry(QModelIndex index)
{
    if ( !index.isValid() ) return;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return;

    int i = index.row();
    beginRemoveRows(QModelIndex(), i, i);
    d->entries.removeAt(i);
    endRemoveRows();
}

void CramFileModel::clear()
{
    beginResetModel();
    d->entries.clear();
    endResetModel();
}

int CramFileModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->entries.count();
}

int CramFileModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant CramFileModel::data(const QModelIndex & index, int role) const
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

bool CramFileModel::setData(const QModelIndex & i, const QVariant & value, int role)
{
    if ( !i.isValid() ) return false;
    if ( i.row() < 0 || i.row() >= d->entries.count() ) return false;

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if ( i.column() == QuestionColumn ) d->entries[ i.row() ].question = value.toString();
        if ( i.column() == AnswerColumn   ) d->entries[ i.row() ].answer   = value.toString();

        //analize last row
        QModelIndex lastQ = index( rowCount()-1, QuestionColumn);
        QModelIndex lastA = index( rowCount()-1, AnswerColumn);
        if (!data(lastQ, Qt::DisplayRole).toString().isEmpty() ||
            !data(lastA, Qt::DisplayRole).toString().isEmpty()) {
            addNewEntry();
        }

        emit dataChanged(i,i);
        return true;
    }

    return false;
}

QVariant CramFileModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CramFileModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() >= d->entries.count() ) return Qt::ItemIsDropEnabled;

    if ( index.column() == IdColumn ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QString CramFileModel::filePath() const
{
    return d->filePath;
}

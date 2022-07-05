
#include <QtXml>
#include <QtGui>
#include <QtCore>
#include <QtSpeech>

#include "TogMeg.h"
#include "TogMegFileModel.h"

#include "IconSet.h"

class QA
{
public:
    QString q;
    QString a;
};

class TogMegFileModel::Private
{
public:
    QList<QA> qas;
    bool isModified;
    QString filePath;
    QPointer<QtSpeech> speechQ;
    QPointer<QtSpeech> speechA;
    QVariant id_speechQ;
    QVariant id_speechA;
    QString name_speechQ;
    QString name_speechA;
};

/*!
 Crestes TogMegFileModel
 */
TogMegFileModel::TogMegFileModel(QObject * parent)
:QAbstractListModel(parent)
{
    d = new Private;
    d->isModified = false;
    try {
        d->speechQ = QtSpeech::Create(this);
        d->speechA = QtSpeech::Create(this);
    }
    catch (QtSpeech::Error & e) {
        qDebug() << e.msg;
    }
    addNewEntry();
}

/*!
 Deletes the object.
 */
TogMegFileModel::~TogMegFileModel()
{
    delete d;
}

bool TogMegFileModel::loadTabFile(QString filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    beginResetModel();
    d->qas.clear();
    d->filePath = filePath;
    bool speechLine = true;

    while (!f.atEnd()) {
        QString line = QString::fromUtf8(f.readLine());
        QStringList args = line.split("\t");
        if (args.count() <2) continue;
        
        static const QString speechPref = "speech:";
        QString argQ = args[0].simplified();
        QString argA = args[1].simplified();

        if (speechLine) {
            bool used = false;
            if (argQ.startsWith(speechPref)) {
                d->id_speechQ = argQ.mid(speechPref.length());
                d->speechQ->setVoice(d->id_speechQ);
                if (d->id_speechQ.isNull())
                    d->name_speechQ.clear();
                else d->name_speechQ = d->speechQ->voiceName();
                used = true;
            }

            if (argA.startsWith(speechPref)) {
                d->id_speechA = argA.mid(speechPref.length());
                d->speechA->setVoice(d->id_speechA);
                if (d->id_speechA.isNull())
                    d->name_speechA.clear();
                else d->name_speechA = d->speechA->voiceName();
                used = true;
            }

            speechLine = false;
            if (used) continue;
        }

        QA entry;
        entry.q = argQ;
        entry.a   = argA;
        d->qas << entry;
    }
    
    endResetModel();
    addNewEntry();
    
    return true;
}

bool TogMegFileModel::loadXmlFile(QString filePath)
{
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    
    beginResetModel();
    d->qas.clear();
    d->filePath = filePath;

    QDomDocument doc;
    doc.setContent(&f);
    
    QDomElement el = doc.documentElement();
    QDomNode n = el.firstChild();
    while (!n.isNull()) {
        QDomElement e_q = n.firstChildElement("question");
        QDomElement e_a = n.firstChildElement("answer");
        
        QA entry;
        entry.q = e_q.firstChild().toText().data();
        entry.a   = e_a.firstChild().toText().data();
        
        d->qas << entry;
        
        n = n.nextSibling();
    }
    
    endResetModel();
    addNewEntry();
    
    return true;
}

bool TogMegFileModel::saveTabFile(QString filePath)
{
    QFile f(filePath);
    if (f.open(QIODevice::WriteOnly)) {
        QString line = "speech:"+d->id_speechQ.toString()+"\t"+
                       "speech:"+d->id_speechA.toString()+"\n";
        f.write(line.toUtf8());

        foreach (QA entry, d->qas) {
            if (entry.q.isEmpty() && entry.a.isEmpty())
                continue;
            
            QString line = entry.q+"\t"+entry.a+"\n";
            f.write(line.toUtf8());
        }
        
        d->filePath = filePath;
        return true;
    }
    
    return false;
}

bool TogMegFileModel::saveXmlFile(QString filePath)
{
    QFile f(filePath);
    if (f.open(QIODevice::WriteOnly)) {
        QDomDocument doc("taskcontentxml");
        
        QDomElement docEl = doc.createElement("content");
        doc.appendChild(docEl);
        
        foreach (QA entry, d->qas) {
            QDomElement entryEl = doc.createElement("studyentry");
            docEl.appendChild(entryEl);
            
            QDomElement questionEl = doc.createElement("question");
            entryEl.appendChild(questionEl);
            
            QDomText questionTextEl = doc.createTextNode( entry.q );
            questionEl.appendChild(questionTextEl);
            
            QDomElement answerEl = doc.createElement("answer");
            entryEl.appendChild(answerEl);
            
            QDomText answerTextEl = doc.createTextNode( entry.a );
            answerEl.appendChild(answerTextEl);
        }
        
        f.write(doc.toByteArray());

        d->filePath = filePath;
        return true;
    }
    
    return false;
}

QModelIndex TogMegFileModel::addNewEntry()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    QA entry;
    d->qas << entry;
    endInsertRows();

    return index(rowCount()-1, ColQ);
}

void TogMegFileModel::removeEntry(QModelIndex index)
{
    if ( !index.isValid() ) return;
    if ( index.row() < 0 || index.row() >= d->qas.count() ) return;

    int row = index.row();
    int idx = row-1;
    beginRemoveRows(QModelIndex(), row, row);
    d->qas.removeAt(idx);
    endRemoveRows();
}

void TogMegFileModel::swapQA(QModelIndex mi)
{
    if ( !mi.isValid() ) return;
    if ( mi.row() < 0 || mi.row() >= d->qas.count() ) return;

    int i = mi.row()-1;

    qSwap(d->qas[i].a, d->qas[i].q);
    emit dataChanged(index(i,0), index(i, ColumnCount));
}

void TogMegFileModel::clear()
{
    beginResetModel();
    d->qas.clear();
    QA entry;
    d->qas << entry;
    endResetModel();
}

int TogMegFileModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return d->qas.count()+1;
}

int TogMegFileModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant TogMegFileModel::data(const QModelIndex & i, int role) const
{
    if ( !i.isValid() ) return QVariant();
    if ( i.row() < 0 || i.row() > d->qas.count() ) return QVariant();

    if ( i.row() == 0 ) {
        if (role == Qt::EditRole) {
            if ( i.column() == ColQ) return d->speechQ->voices();
            if ( i.column() == ColA) return d->speechA->voices();
        }
        else if (role == Qt::DisplayRole) {
            if ( i.column() == ColQ) {
                if (d->name_speechQ.isEmpty())
                    return tr("Choose questioning voice...");
                return d->name_speechQ;
            }
            if ( i.column() == ColA) {
                if (d->name_speechA.isEmpty())
                    return tr("Choose answering voice...");
                return d->name_speechA;
            }
        }
        else if (role == Qt::DecorationRole && i.column() == ColN) {
            return EmbIcon("Microphone");
        }
        else if (role == Qt::BackgroundRole) {
            return QColor(235,255,235);
        }
    }
    else {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if ( i.column() == ColN) return QString("  %1 ").arg(i.row());
            if ( i.column() == ColQ ) return d->qas[ i.row()-1 ].q;
            if ( i.column() == ColA ) return d->qas[ i.row()-1 ].a;
        }
    }

    if (role == Qt::ForegroundRole) {
        if (i.column() == ColN) return QColor(128,128,128);
    }

    if (role == Qt::TextAlignmentRole) {
        if (i.column() == ColN) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (i.column() == ColQ) return int(Qt::AlignCenter);
        if (i.column() == ColA) return int(Qt::AlignCenter);
    }

    if (role == Qt::FontRole && i.row() == 0) {
        QFont f;
        f.setItalic(true);
        return f;
    }

    return QVariant();
}

bool TogMegFileModel::setData(const QModelIndex & i, const QVariant & v, int role)
{
    if ( !i.isValid() ) return false;
    if ( i.row() < 0 || i.row() > d->qas.count() ) return false;

    if (i.row() == 0) {
        if ( i.column() == ColQ) {
            d->id_speechQ = v;
            d->speechQ->setVoice(v);
            if (v.isNull())
                d->name_speechQ.clear();
            else d->name_speechQ = d->speechQ->voiceName();
            return true;
        }
        if ( i.column() == ColA) {
            d->id_speechA = v;
            d->speechA->setVoice(v);
            if (v.isNull())
                d->name_speechA.clear();
            else d->name_speechA = d->speechA->voiceName();
            return true;
        }

        return false;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        bool change = false;
        QA & e = d->qas[ i.row()-1 ];
        if ( i.column() == ColQ ) {
            if (e.q != v.toString()) {
                e.q = v.toString();
                change = true;
            }
        }
        if ( i.column() == ColA ) {
            if (e.a != v.toString()) {
                e.a = v.toString();
                change = true;
            }
        }

        //analize last row
        QModelIndex lastQ = index( rowCount()-1, ColQ);
        QModelIndex lastA = index( rowCount()-1, ColA);
        if (!data(lastQ, Qt::DisplayRole).toString().isEmpty() ||
            !data(lastA, Qt::DisplayRole).toString().isEmpty()) {
            addNewEntry();
        }

        if (change) emit dataChanged(i,i);
        return change;
    }

    else if (role == SpeechRole) {
        if ( i.column() == ColQ ) d->speechQ->say(d->qas[ i.row()-1 ].q);
        if ( i.column() == ColA ) d->speechA->say(d->qas[ i.row()-1 ].a);
    }

    return false;
}

QVariant TogMegFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole) {
        if ( section == ColN)
            return tr("#");
        else if ( section == ColQ)
            return tr("Question");
        else if ( section == ColA )
            return tr("Answer");
    }

    if (role == Qt::TextAlignmentRole) {
        if (section == ColN) return int(Qt::AlignRight | Qt::AlignVCenter);
        if (section == ColQ) return int(Qt::AlignCenter);
        if (section == ColA) return int(Qt::AlignCenter);
    }

    return QVariant();
}

Qt::ItemFlags TogMegFileModel::flags(const QModelIndex &index) const 
{
    if ( !index.isValid() ) return Qt::ItemIsDropEnabled;
    if ( index.row() < 0 || index.row() > d->qas.count() ) return Qt::ItemIsDropEnabled;

    if ( index.column() == ColN ) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QString TogMegFileModel::filePath() const
{
    return d->filePath;
}

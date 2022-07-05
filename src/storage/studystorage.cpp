// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#include <QtSql>
#include "studystorage.h"

class StudyStorage::Private {
public:
};

/*!
 * Creates the object.
 */
StudyStorage::StudyStorage(QObject * parent)
:QObject(parent)
{
    d = new Private;
    QSqlQuery query;
    query.exec("create table test( "\
               " id          integer primary key autoincrement, "\
               " tid         integer, "\
               " question    varchar(400), "\
               " answer      varchar(400), "\
               " msecs       integer, "\
               " hits        integer, "\
               " misses      integer, "\
               " score       integer "\
               ")");
}

/*!
 * Deletes it.
 */
StudyStorage::~StudyStorage()
{
    delete d;
}

StudyEntry * StudyStorage::nextEntry(int testID)
{
    QSqlQuery query;

    query.exec("SELECT count() FROM test WHERE tid="+QString::number(testID));
    query.first();
    int count = query.value(0).toInt(); 
    if ( !count ) return 0L;

    query.exec("SELECT min(score) FROM test WHERE tid="+QString::number(testID));
    query.first();
    int min = query.value(0).toInt(); 
    
    query.exec("SELECT max(score) FROM test WHERE tid="+QString::number(testID));
    query.first();
    int max = query.value(0).toInt(); 

    qDebug() << min << max;
    
    query.exec("SELECT id, question, answer, score, misses, hits FROM test WHERE tid="+QString::number(testID));
    int pos = random() % count;
    
    StudyEntry * entry = new StudyEntry;
    if (query.seek(pos)) {
        entry->id = query.value(0).toInt();
        entry->question = query.value(1).toString();
        entry->answer   = query.value(2).toString();
        entry->score    = query.value(3).toInt();
        entry->misses   = query.value(4).toInt();
        entry->hits     = query.value(5).toInt();
    }

    return entry;
}

void StudyStorage::updateEntry(StudyEntry * entry)
{
    QSqlQuery query;
    query.prepare("UPDATE test SET score=:score, hits=:hits, misses=:misses WHERE id="+QString::number(entry->id));
    query.bindValue(":score",  entry->score);
    query.bindValue(":misses", entry->misses);
    query.bindValue(":hits",   entry->hits);
    query.exec();
}

void StudyStorage::closeEntry(StudyEntry * entry)
{
    delete entry;
}


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

StudyEntry * StudyStorage::nextEntry()
{
    QSqlQuery query;

    query.exec("SELECT count() from test");
    query.first();
    int count = query.value(0).toInt(); 
    if ( !count ) return 0L;
    
    query.exec("SELECT question, answer FROM test");
    int pos = random() % count;
    
    StudyEntry * entry = new StudyEntry;
    if (query.seek(pos)) {
        entry->question = query.value(0).toString();
        entry->answer = query.value(1).toString();
    }

    return entry;
}

void StudyStorage::closeEntry(StudyEntry * entry)
{
    delete entry;
}


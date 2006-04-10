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
    StudyEntry * entry = new StudyEntry;
    entry->question = "question1";
    entry->answer = "answer1";

    return entry;
}

void StudyStorage::closeEntry(StudyEntry * entry)
{
    delete entry;
}


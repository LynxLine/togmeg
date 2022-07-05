// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2005

#ifndef STUDYSTORAGE_H
#define STUDYSTORAGE_H

#include <QObject>

/*!
 * \class StudyStorage
 */
class StudyEntry {
public:
    int id;
    QString question;
    QString answer;

    int score;
    int misses;
    int hits;
};

/*!
 * \class StudyStorage
 */
class StudyStorage : public QObject
{
Q_OBJECT
public:
    StudyStorage(QObject * parent = 0);
    ~StudyStorage();

    StudyEntry * nextEntry(int testID);
    void updateEntry(StudyEntry *);
    void closeEntry(StudyEntry *);


private:
    class Private;
    Private * d;
};

#endif // STUDYSTORAGE_H


//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "examinator.h"

#include "studytask.h"
#include "tasklistmodel.h"

class Examinator::Private {
public:
    StudyTask * task;
};

/*!
 * Creates the object.
 */
Examinator::Examinator(QObject * parent)
:QObject(parent)
{
    d = new Private;
    d->task = 0L;
}

/*!
 * Deletes it.
 */
Examinator::~Examinator()
{
    qDebug() << "~Examinator()";
    delete d;
}

void Examinator::setCurrentTask(QString taskId)
{
    qDebug() << "Examinator::setCurrentTask()" << taskId;

    if ( taskId.isEmpty() ) {
        d->task = 0L;
        return;
    }

    d->task = TaskListModel::instance()->task( taskId );
    if ( !d->task ) return;
}

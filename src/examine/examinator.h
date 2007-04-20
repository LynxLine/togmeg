//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef EXAMINATOR_H
#define EXAMINATOR_H

#include <QObject>

/*!
 * \class Examinator
 */
class Examinator : public QObject
{
Q_OBJECT
public:
    Examinator(QObject * parent = 0);
    virtual ~Examinator();

public slots:
    void setCurrentTask(QString taskId);

private:
    class Private;
    Private * d;
};

#endif // EXAMINATOR_H

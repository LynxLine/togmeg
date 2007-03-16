//
// Copyright (C) 2006 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef LOGGER_H
#define LOGGER_H
 
#include <QObject>

class Logger : QObject
{
Q_OBJECT
public:
    Logger(QObject * parent);
    virtual ~Logger();
    static Logger * instance();

    void messageHandler(QtMsgType type, const char * msg);

private:
    class Private;
    Private * d;
};

#endif // LOGGER_H

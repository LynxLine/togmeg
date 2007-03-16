//
// Copyright (C) 2006 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtCore>
#include "logger.h"

class Logger::Private
{
public:
    static Logger * instance;

    QString logPath;
    QPointer<QFile> logFile;
};

Logger * Logger::Private::instance = 0L;

Logger::Logger(QObject * parent)
:QObject(parent)
{
	d = new Private;
    Logger::Private::instance = this;

    d->logPath = QDir(QDir::tempPath()).absolutePath() +QDir::separator();
    d->logPath +=  "serrater.log";

    d->logFile = new QFile( d->logPath, this );
    d->logFile->open(QIODevice::WriteOnly);
}

Logger::~Logger()
{
    if ( d->logFile ) {
        d->logFile->close();
        d->logFile->remove();
    }

	delete d;
}

Logger * Logger::instance()
{
    return Logger::Private::instance;
}

void Logger::messageHandler(QtMsgType type, const char * msg)
{
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        break;
    }

    d->logFile->write( QDateTime::currentDateTime().toString().toUtf8() );
    d->logFile->write( ": " );
    d->logFile->write( QString::fromLocal8Bit(msg).toUtf8() );
    d->logFile->write( "\r\n" );
    d->logFile->flush();
}

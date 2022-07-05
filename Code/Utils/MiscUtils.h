//
// Copyright Petrostreamz 2009
//

#ifndef MiscUtils_H
#define MiscUtils_H

#include <QString>

class MiscUtils
{
public:
	static QString buildId();
    static QString userName();
    static QString hostName();
    static QString userAtHostName();
    static void moveFileToTrash(QString filePath);
    static void showInFileManager(QString filePath);
    static void copyFolder(QString sourcePath, QString destPath);
    static QStringList splitWithRespectQuotes(QString);
    static QString uniqueId(int length = 20);
    static bool isProcessRunning(qint64 pid);
};

#endif // MiscUtils_H

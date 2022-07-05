//
// Copyright Oleksandr Iakovliev, 2010
//

#include <QtCore>
#include "BaseProject.h"

class BaseProject::Private {
public:
    Private():isModified(false) {}
    
    QString filePath;
    bool isModified;
};

BaseProject::BaseProject(QObject * parent)
:QObject(parent)
{
    d = new Private;
}

BaseProject::~BaseProject()
{
    delete d;
}

QString BaseProject::fileName() const
{
    if (filePath().isEmpty()) 
        return QString::null;
    
    return QFileInfo(filePath()).fileName();
}

QString BaseProject::filePath() const {
    return d->filePath;
}

QString BaseProject::folderPath() const
{
    if (filePath().isEmpty()) 
        return QString::null;
    
    QString path = QFileInfo(filePath()).path();
    if (!path.endsWith("/")) path += "/";
    return path;
}

QString BaseProject::extension() const {
    return "txt";
}

QStringList BaseProject::extensions() const {
    return QStringList() << extension();
}

QString BaseProject::fileDialogFilter() const {
    return tr("Text files (*.txt);;Any file (*)");
}

bool BaseProject::isModified() const {
    return d->isModified;
}

void BaseProject::setModified(bool f)
{
    if (f == isModified()) return;
    d->isModified = f;
    emit modifiedStatusChanged(f);
}

bool BaseProject::loadFile(QString filePath)
{    
    setFilePath(filePath);
    setModified(false);    

    emit loadCompleted();
    return true;
}

bool BaseProject::saveFile(QString filePath)
{
    setFilePath(filePath);
    setModified(false);    
    
    emit saveCompleted();    
    return true;
}

void BaseProject::setFilePath(QString newFilePath)
{
    QString oldFolderPath = folderPath();
    QString oldFilePath = filePath();
    
    d->filePath = newFilePath;
    QDir::setCurrent(folderPath());
    
    if (oldFilePath != filePath()) emit filePathChanged(filePath());
    if (oldFolderPath != folderPath()) emit folderPathChanged(folderPath());
}

void BaseProject::clear()
{
    d->filePath.clear();
    setModified(false);
}

//
// Copyright Oleksandr Iakovliev, 2010
//

#include <QtCore>
#include "CrammeroProject.h"

#include "CramFileModel.h"

class CrammeroProject::Private {
public:
    QPointer<CramFileModel> model;
};

CrammeroProject::CrammeroProject(QObject * parent)
:BaseProject(parent)
{
    d = new Private;
}

CrammeroProject::~CrammeroProject()
{
    delete d;
}

bool CrammeroProject::loadFile(QString filePath)
{    
    QFile file( filePath );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return false;
    
    clear();
    setFilePath(filePath);
    QDir::setCurrent(folderPath());
    
    /*
    QTime time = QTime::currentTime();
    
    QString log;
    QTextStream out(&log);
    XML_Reader reader(this, CrammeroProject::Xml::Tag, file, &out);
    XML_Reader::ErrorLevel err = reader.Read_XML();
    
    if ( err > XML_Reader::WARNING ) {
        qDebug() << reader.errorString();
        return false;
    }
     */
    
    //qDebug() << "Project loading time(msecs):" << time.msecsTo(QTime::currentTime());
    
    return BaseProject::loadFile(filePath);
}

bool CrammeroProject::saveFile(QString filePath)
{
    setFilePath(filePath);
    
    /*
    QFile file(filePath);    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    
    Write_Project_XML(file);
     */
    
    return BaseProject::saveFile(filePath);
}

void CrammeroProject::clear()
{
    model()->clear();
    BaseProject::clear();
}

/*!
 Factory method to create model object.
 */
CramFileModel * CrammeroProject::createModel() const
{
    return new CramFileModel(const_cast<CrammeroProject *>(this));
}

CramFileModel * CrammeroProject::model() const
{
    if (!d->model)
        d->model = createModel();
    return d->model;
}

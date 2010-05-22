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
    
    bool ok = false;
    if (filePath.endsWith(".xml", Qt::CaseInsensitive))
        ok = model()->saveXmlFile(filePath);
    else ok = model()->saveTabFile(filePath);    
    if (!ok) return false;
    
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

// Extensions and file dialog flters

QString CrammeroProject::extension() const {
    return "tab";
}

QStringList CrammeroProject::extensions() const {
    return QStringList() << "tab" << "xml";
}

QString CrammeroProject::fileDialogFilter() const {
    return tr("Tab delimited files (*.tab);;Xml files (*.xml);;Any file (*)");
}

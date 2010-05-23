//
// Copyright Oleksandr Iakovliev, 2010
//

#include <QtCore>
#include "TogMegProject.h"

#include "CramFileModel.h"

class TogMegProject::Private {
public:
    QPointer<CramFileModel> model;
};

TogMegProject::TogMegProject(QObject * parent)
:BaseProject(parent)
{
    d = new Private;
}

TogMegProject::~TogMegProject()
{
    delete d;
}

bool TogMegProject::loadFile(QString filePath)
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
    XML_Reader reader(this, TogMegProject::Xml::Tag, file, &out);
    XML_Reader::ErrorLevel err = reader.Read_XML();
    
    if ( err > XML_Reader::WARNING ) {
        qDebug() << reader.errorString();
        return false;
    }
     */
    
    //qDebug() << "Project loading time(msecs):" << time.msecsTo(QTime::currentTime());
    
    return BaseProject::loadFile(filePath);
}

bool TogMegProject::saveFile(QString filePath)
{
    setFilePath(filePath);
    
    bool ok = false;
    if (filePath.endsWith(".xml", Qt::CaseInsensitive))
        ok = model()->saveXmlFile(filePath);
    else ok = model()->saveTabFile(filePath);    
    if (!ok) return false;
    
    return BaseProject::saveFile(filePath);
}

void TogMegProject::clear()
{
    model()->clear();
    BaseProject::clear();
}

/*!
 Factory method to create model object.
 */
CramFileModel * TogMegProject::createModel() const
{
    return new CramFileModel(const_cast<TogMegProject *>(this));
}

CramFileModel * TogMegProject::model() const
{
    if (!d->model)
        d->model = createModel();
    return d->model;
}

// Extensions and file dialog flters

QString TogMegProject::extension() const {
    return "tab";
}

QStringList TogMegProject::extensions() const {
    return QStringList() << "tab" << "xml";
}

QString TogMegProject::fileDialogFilter() const {
    return tr("Tab delimited files (*.tab);;Xml files (*.xml);;Any file (*)");
}

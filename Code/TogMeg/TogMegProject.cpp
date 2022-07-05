
#include <QtGui>
#include "TogMegProject.h"
#include "QAModel.h"

class TogMegProject::Private {
public:
    QPointer<QAModel> model;
};

TogMegProject::TogMegProject(QObject * parent)
:BaseProject(parent)
{
    d = new Private;
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if (!path.endsWith("/")) path += "/";
    path += "Library";

    if (!QFileInfo(path).exists()) {
        QDir dir(QDir::homePath());
        dir.mkpath(path);
    }
    QDir::setCurrent(path);
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
    
    bool ok = false;
    if (filePath.endsWith(".xml", Qt::CaseInsensitive))
        ok = model()->loadXmlFile(filePath);
    else ok = model()->loadTabFile(filePath);
    if (!ok) return false;
    
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
QAModel * TogMegProject::createModel() const
{
    return new QAModel(const_cast<TogMegProject *>(this));
}

QAModel * TogMegProject::model() const
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
    return QStringList() << "tab"; // << "xml";
}

QString TogMegProject::fileDialogFilter() const {
    //return tr("Tab delimited files (*.tab);;Xml files (*.xml);;Any file (*)");
    //return tr("Tab delimited files (*.tab);;Any file (*)");
    return tr("Tab delimited files (*.tab)");
}

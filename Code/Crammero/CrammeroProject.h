//
// Copyright Oleksandr Iakovliev, 2010
//

#ifndef TogMegPoject_H
#define TogMegPoject_H

#include "BaseProject.h"

class CramFileModel;

class TogMegProject : public BaseProject
{		
Q_OBJECT
friend class CramFileModel;
public:  
    TogMegProject(QObject * parent);
    virtual ~TogMegProject();
    
    CramFileModel * model() const;
      
    virtual QString extension() const;
    virtual QStringList extensions() const;
    virtual QString fileDialogFilter() const;
    
public slots:
    virtual bool loadFile(QString filePath);
    virtual bool saveFile(QString filePath);
    virtual void clear();

protected:
    virtual CramFileModel * createModel() const;
    
private:
    class Private;
    Private * d;
};

#endif // TogMegPoject_H

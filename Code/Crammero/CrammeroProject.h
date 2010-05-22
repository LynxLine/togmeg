//
// Copyright Oleksandr Iakovliev, 2010
//

#ifndef CrammeroPoject_H
#define CrammeroPoject_H

#include "BaseProject.h"

class CramFileModel;

class CrammeroProject : public BaseProject
{		
Q_OBJECT
friend class CramFileModel;
public:  
    CrammeroProject(QObject * parent);
    virtual ~CrammeroProject();
    
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

#endif // CrammeroPoject_H

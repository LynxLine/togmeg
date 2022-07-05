
#ifndef TogMegPoject_H
#define TogMegPoject_H

#include "BaseProject.h"

class QAModel;

class TogMegProject : public BaseProject
{		
Q_OBJECT
friend class QAModel;
public:  
    TogMegProject(QObject * parent);
    virtual ~TogMegProject();
    
    QAModel * model() const;
      
    virtual QString extension() const;
    virtual QStringList extensions() const;
    virtual QString fileDialogFilter() const;
    
public slots:
    virtual bool loadFile(QString filePath);
    virtual bool saveFile(QString filePath);
    virtual void clear();

protected:
    virtual QAModel * createModel() const;
    
private:
    class Private;
    Private * d;
};

#endif // TogMegPoject_H

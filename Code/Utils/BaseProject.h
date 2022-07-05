//
// Copyright Oleksandr Iakovliev, 2010
//

#ifndef BaseProject_H
#define BaseProject_H

#include <QObject>

class BaseProject : public QObject 
{		
Q_OBJECT
public:  
    BaseProject(QObject * parent);
    virtual ~BaseProject();
    
    QString fileName() const;
    QString filePath() const;
    QString folderPath() const;
            
    virtual QString extension() const;
    virtual QStringList extensions() const;
    virtual QString fileDialogFilter() const;
    
    bool isModified() const;    

public slots:
    virtual void setModified(bool=true);
    virtual void setFilePath(QString);
    virtual bool loadFile(QString filePath);
    virtual bool saveFile(QString filePath);
    virtual void clear();
    
signals:
    //! Emits on project file change
    void filePathChanged(const QString &);
    void folderPathChanged(const QString &);
    void modifiedStatusChanged(bool);
    void loadCompleted();
    void saveCompleted();
    
private:
    class Private;
    Private * d;
};

#endif // BaseProject_H

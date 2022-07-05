//
// Copyright Petrostreamz 2009
//

#ifndef SOFTWAREUPDATE_H
#define SOFTWAREUPDATE_H

#include <QObject>
#include <QDialog>
#include <QNetworkReply>

class Application;

class SoftwareUpdate : public QObject
{
    Q_OBJECT
public:
    SoftwareUpdate(QObject * parent);
    virtual ~SoftwareUpdate();
    
    void checkUpdates();
    
    QString appCastUrl() const;
    void setAppCastUrl(QString);
    
    QPixmap appLogo() const;
    void setAppLogo(QPixmap);
    
    bool isVersionSkip() const;
    void setVersionSkip(bool);

signals:
    void applicationCloseRequest(bool & accepted);
    void reportUpToDated(bool ok, QString err);
    void updateFound(QString version, 
                     QString description, 
                     QString url);
    
private slots:
    void dataReady();
    void dataError(QNetworkReply::NetworkError);
    void showUpdateDialog(QString, QString, QString);
    
    void startDownload(QString url);
    void downloadingUpdate();
    void downloadDone();
    void cancelUpdate();
    void installUpdate();

private:
    class Private;
    Private * d;
};

class SoftwareUpdateDialog : public QDialog
{
    Q_OBJECT
public:
    SoftwareUpdateDialog(QWidget * parent);
    virtual ~SoftwareUpdateDialog();

    void setPixmap(QPixmap);
    void setVersion(QString);
    void setText(QString);
    
private slots:
    void buttonClicked(QAbstractButton *);
    
private:
    class Private;
    Private * d;
};

class SoftwareUpdateProgressDialog : public QDialog
{
    Q_OBJECT
public:
    SoftwareUpdateProgressDialog(QWidget * parent);
    virtual ~SoftwareUpdateProgressDialog();
   
signals:
    void install();
    void cancelled();
    
public slots:
    void setCompleted();
    void setError(QString);
    void setProgress(qint64 done, qint64 total);
    void actionButtonClicked();
    
private:
    class Private;
    Private * d;
};

#endif // SOFTWAREUPDATE_H

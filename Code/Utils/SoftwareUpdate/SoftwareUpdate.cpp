//
// Copyright Petrostreamz 2009
//

#include <QtGui>
#include <QtXml>
#include <QtNetwork>

#ifdef Q_WS_WIN
#include <qt_windows.h>
#endif

#include "SoftwareUpdate.h"
#include "ui_SoftwareUpdate.h"
#include "ui_SoftwareUpdateProgress.h"

#include "AppStyles.h"
#include "MiscUtils.h"

class SoftwareUpdate::Private {
public:
    QPointer<QNetworkAccessManager> netManager;
    
    QString version;
    QPixmap appLogo;
    QString appCastUrl;
    bool versionSkipActive;

    QString updateFilePath;
    QPointer<QFile> updateFile;
    QPointer<QNetworkReply> download;
    QPointer<SoftwareUpdateProgressDialog> progressDlg;
};

SoftwareUpdate::SoftwareUpdate(QObject * parent)
:QObject(parent)
{
    d = new Private;
    d->versionSkipActive = true;
    d->netManager = new QNetworkAccessManager(this);
    
    connect(this, SIGNAL(updateFound(QString, QString, QString)),
            this, SLOT(showUpdateDialog(QString, QString, QString)), Qt::QueuedConnection);
}

SoftwareUpdate::~SoftwareUpdate()
{
    delete d;
}

QString SoftwareUpdate::appCastUrl() const
{
    return d->appCastUrl;
}

void SoftwareUpdate::setAppCastUrl(QString url)
{
    d->appCastUrl = url;
}

QPixmap SoftwareUpdate::appLogo() const
{
    return d->appLogo;
}

void SoftwareUpdate::setAppLogo(QPixmap pm)
{
    d->appLogo = pm;
}

bool SoftwareUpdate::isVersionSkip() const
{
    return d->versionSkipActive;
}

void SoftwareUpdate::setVersionSkip(bool f)
{
    d->versionSkipActive = f;
}

void SoftwareUpdate::checkUpdates()
{
#ifdef Q_WS_WIN
    QSettings s(QSettings::NativeFormat, QSettings::SystemScope,
                qApp->organizationName(), qApp->applicationName());

    QString expectedAppPath = s.value("InstallPath00").toString();
    if (!expectedAppPath.endsWith("\\")) expectedAppPath += "\\";
    expectedAppPath += qApp->applicationName()+".exe";

    QString executedAppPath = QDir::toNativeSeparators(qApp->applicationFilePath());
    QString executedBaseName = QFileInfo(qApp->applicationFilePath()).completeBaseName();

    if (expectedAppPath.toLower() != executedAppPath.toLower()) {
        //This version of application may be another installation
        //Let's try to check if it is installation and update
        QDir d(qApp->applicationDirPath());
        bool ok = d.exists("QtCore4.dll") &&
                    d.exists("QtGui4.dll") &&
                    d.exists(QString("Uninstall_%1.exe").arg(qApp->applicationName()));

        if (qApp->applicationName() != executedBaseName) {
            emit reportUpToDated(false, tr("You executed renamed executable, X402"));
            return;
        }

        if (!ok) {
            emit reportUpToDated(false, tr("Application is not in installation location, X403"));
            return;
        }

        //Update registry variable to run installer in proper way
        s.setValue("InstallPath00", executedAppPath);
        s.setValue("Version", MiscUtils::buildId());
        s.sync();

        { // check that it is written (HKLM registry can be read-only)
            QSettings s1(QSettings::NativeFormat, QSettings::SystemScope,
                         qApp->organizationName(), qApp->applicationName());
            if (s1.value("InstallPath00").toString() != executedAppPath) {
                emit reportUpToDated(false, tr("Can not write InstallPath00 in registry, "\
                                               "Admin rights are probably needed, X490"));
                return;
            }
        }
	}

    QNetworkRequest request;
    request.setUrl(QUrl(appCastUrl()));
	QByteArray id = MiscUtils::userAtHostName().toUtf8().toBase64();
	QString agent = tr("%1 %2 %3").arg(qApp->applicationName())
                                  .arg(MiscUtils::buildId())
                                  .arg(QString::fromUtf8(id));
    request.setRawHeader("User-Agent", agent.toUtf8());
    
    QNetworkReply * reply = d->netManager->get(request);
    connect(reply, SIGNAL(readChannelFinished()), 
            this, SLOT(dataReady()), Qt::QueuedConnection);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), 
            this, SLOT(dataError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
#endif
}

void SoftwareUpdate::dataError(QNetworkReply::NetworkError err)
{
    QNetworkReply * reply = dynamic_cast<QNetworkReply *>(sender());
    if (!reply) {
        emit reportUpToDated(false, tr("App problem, X444"));
        return;
    }

    emit reportUpToDated(false, tr("Network Error: %1, %2")
        .arg(err)
        .arg(reply->errorString())
    );
}

void SoftwareUpdate::dataReady()
{
    QNetworkReply * reply = dynamic_cast<QNetworkReply *>(sender());
    if (!reply) {
        emit reportUpToDated(false, tr("App problem, X445"));
        return;
    }
    
    QDomDocument doc;
    if (!doc.setContent(reply)) {
        qDebug() << "doc.setContent() failed";
        emit reportUpToDated(false, tr("Server reply is broken, X401"));
        return;
    }
    QDomElement rss = doc.firstChildElement("rss");
    QDomElement channel = rss.firstChildElement("channel");
    QDomElement item = channel.lastChildElement("item");
    d->version = item.firstChildElement("title").text();
    QString descr = item.firstChildElement("description").text();
    
    QDomElement enc = item.firstChildElement("enclosure");
    QString url = enc.attribute("url");
    
    emit updateFound(d->version, descr, url);
}

void SoftwareUpdate::showUpdateDialog(QString version, QString descr, QString url)
{
    version = version.left(8);
    QString buildId = MiscUtils::buildId().left(8);

    QSettings s;
    QString skipId = s.value("SkippedVersionOfUpdate", "20090101").toString();

    bool ok = false;
    int oldId = buildId.toInt(&ok);
    if (!ok) {
        emit reportUpToDated(false, tr("Wrong version number handled, X446"));
        return;
    }
    
    int id = version.toInt(&ok);
    if (!ok) {
        emit reportUpToDated(false, tr("Wrong version number reported, X447"));
        return;
    }
    
    int skip = skipId.toInt(&ok);
    if (!ok) skip = 10000101;
    
    if (oldId >= id) {
        emit reportUpToDated(true, tr("You have the latest version of the Software."));
        return;
    }

    if (d->versionSkipActive && skip >= id) return;
    
    QWidget * top = qApp->activeWindow();
    if (!top) {
        foreach(top, QApplication::topLevelWidgets()) { 
            if (top->isHidden()) continue;
            break;
        }
    }
    
    SoftwareUpdateDialog dlg(top);
    dlg.setPixmap(appLogo());
    dlg.setVersion(version);
    dlg.setText(descr);
    
    if (dlg.exec())
        startDownload(url);
}

void SoftwareUpdate::startDownload(QString url)
{
    d->updateFilePath = QDir::tempPath();
    d->updateFilePath = d->updateFilePath.replace("/", QDir::separator());
    d->updateFilePath += QDir::separator();
    d->updateFilePath += QString("update-%1-%2-%3.exe")
        .arg(qApp->applicationName())
        .arg(d->version)
        .arg(MiscUtils::uniqueId(6));
    
    d->updateFile = new QFile(d->updateFilePath);
    d->updateFile->open( QIODevice::WriteOnly );
        
    QNetworkRequest request;
    request.setUrl(url);
	QByteArray id = MiscUtils::userAtHostName().toUtf8().toBase64();
    QString agent = tr("%1 %2 %3").arg(qApp->applicationName())
                                  .arg(MiscUtils::buildId())
                                  .arg(QString::fromUtf8(id));
    request.setRawHeader("User-Agent", agent.toUtf8());
    
    d->download = d->netManager->get(request);
    connect(d->download, SIGNAL(readyRead()),
            this,   SLOT(downloadingUpdate()));
    connect(d->download, SIGNAL(finished()),
            this, SLOT(downloadDone()));

    QWidget * top = qApp->activeWindow();
    if (!top) {
        foreach(top, QApplication::topLevelWidgets()) { 
            if (top->isHidden()) continue;
            break;
        }
    }
    
    d->progressDlg = new SoftwareUpdateProgressDialog(top);
    connect(d->download, SIGNAL(downloadProgress(qint64, qint64)),
            d->progressDlg, SLOT(setProgress(qint64, qint64)));
    connect(d->progressDlg, SIGNAL(install()), this, SLOT(installUpdate()), Qt::QueuedConnection);
    connect(d->progressDlg, SIGNAL(cancelled()), this, SLOT(cancelUpdate()));
    d->progressDlg->show();
}

void SoftwareUpdate::downloadingUpdate()
{
    QNetworkReply * reply = dynamic_cast<QNetworkReply *>(sender());
    if (!reply) return;
    
    qint64 byteCount = reply->bytesAvailable();
    
    QByteArray bytes;
    bytes.resize( byteCount );
    reply->read( bytes.data(), byteCount );
    
    if (d->updateFile)
        if (d->updateFile->isOpen())
            d->updateFile->write( bytes.data(), byteCount );
}

void SoftwareUpdate::downloadDone()
{
    QNetworkReply * reply = dynamic_cast<QNetworkReply *>(sender());
    if (!reply) return;

    if (!d->updateFile)
        return;
    
    reply->close();
    d->updateFile->close();
    d->updateFile->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        d->progressDlg->setError(reply->errorString());
        return;
    }
    
    d->progressDlg->setCompleted();
    d->progressDlg->actionButtonClicked();
}

void SoftwareUpdate::cancelUpdate()
{
    d->download->abort();
}

void SoftwareUpdate::installUpdate()
{
    bool closed = false;
    emit applicationCloseRequest(closed);
    if (closed) {
#ifdef Q_WS_WIN
        SHELLEXECUTEINFO   sei;
        ZeroMemory ( &sei, sizeof(sei) );

        sei.cbSize          = sizeof(SHELLEXECUTEINFOW);
        sei.hwnd            = NULL;
        sei.fMask           = SEE_MASK_ASYNCOK | SEE_MASK_NOCLOSEPROCESS;
        sei.lpFile          = (WCHAR*)d->updateFilePath.utf16();
        sei.nShow           = SW_SHOWNORMAL;

        ShellExecuteExW(&sei);
#endif
    }
}

//
// SoftwareUpdateDialog
//

class SoftwareUpdateDialog::Private {
public:
    Ui::SoftwareUpdateDialog ui;
    QString version;
};

SoftwareUpdateDialog::SoftwareUpdateDialog(QWidget * parent)
:QDialog(parent, Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint)
{
    d = new Private;
    d->ui.setupUi(this);
    
    d->ui.buttons->button(QDialogButtonBox::Save)->setText(tr("Install Update"));
    d->ui.buttons->button(QDialogButtonBox::Cancel)->setText(tr("Remind Me Later"));
    d->ui.buttons->button(QDialogButtonBox::Discard)->setText(tr("Skip This Version"));

    d->ui.buttons->button(QDialogButtonBox::Save)->setMinimumWidth(150);
    d->ui.buttons->button(QDialogButtonBox::Cancel)->setMinimumWidth(150);
    d->ui.buttons->button(QDialogButtonBox::Discard)->setMinimumWidth(150);  
    
    connect(d->ui.buttons, SIGNAL(clicked(QAbstractButton *)),
            this, SLOT(buttonClicked(QAbstractButton *)));
    
    setWindowIcon(QIcon());
}

SoftwareUpdateDialog::~SoftwareUpdateDialog()
{
    delete d;
}

void SoftwareUpdateDialog::setPixmap(QPixmap pm)
{
    d->ui.l_pixmap->setPixmap(pm);
}

void SoftwareUpdateDialog::setVersion(QString version)
{
    QString data = tr("<b>New version of %1 is available!</b><br><br>"\
                      "%2 %3 is available - you have %4.<br>"\
                      "Would you like to download it now?")
                        .arg(qApp->applicationName())
                        .arg(qApp->applicationName());
    
    Application * app;
    app = (Application *)qApp;
    data = data.arg(version).arg(MiscUtils::buildId());
    d->ui.l_updateInfo->setText(data);
    d->version = version;
}

void SoftwareUpdateDialog::setText(QString text)
{
    d->ui.tb_info->setHtml(text);
}

void SoftwareUpdateDialog::buttonClicked(QAbstractButton * button)
{
    if (button != d->ui.buttons->button(QDialogButtonBox::Discard))
        return;
    
    QSettings s;
    s.setValue("SkippedVersionOfUpdate", d->version);
    
    reject();
}

//
// SoftwareUpdateProgressDialog
//

class SoftwareUpdateProgressDialog::Private {
public:
    Private()
    :completed(false),
     failed(false)
    {;}
    Ui::SoftwareUpdateProgressDialog ui;
    QString version;
    
    bool completed;
    bool failed;
};

SoftwareUpdateProgressDialog::SoftwareUpdateProgressDialog(QWidget * parent)
:QDialog(parent)
{
    d = new Private;
    d->ui.setupUi(this);
    setWindowIcon(QIcon());
    
    setProgress(0, 0);
    connect(d->ui.pb_action, SIGNAL(clicked()),
            this, SLOT(actionButtonClicked()));
}

SoftwareUpdateProgressDialog::~SoftwareUpdateProgressDialog()
{
    delete d;
}

void SoftwareUpdateProgressDialog::setCompleted()
{
    d->completed = true;
    d->ui.l_message->setText(tr("Download operation is completed"));
    d->ui.pb_action->setText(tr("Close"));
}

void SoftwareUpdateProgressDialog::setError(QString err)
{
    d->failed = true;
    d->ui.l_message->setFont(AppStyles::systemFont());
    d->ui.l_message->setText(err);
    d->ui.pb_action->setText(tr("Close"));
}

void SoftwareUpdateProgressDialog::setProgress(qint64 done, qint64 total)
{
    if (total <= 0)        
        d->ui.progress->setValue(0);
    else d->ui.progress->setValue(done*100/total);
}

void SoftwareUpdateProgressDialog::actionButtonClicked()
{
    if (d->completed) {
        emit install();
    }
    if (d->failed) {
        // do nothing, just close
    }
    else {
        // still in progress
        emit cancelled();
    }
    
    deleteLater();
}

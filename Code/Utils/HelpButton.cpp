//
// Copyright Petrostreamz 2007
//

#include <QtGui>
#include "HelpButton.h"

class HelpButton::Private {
public:
    QString keyword;
};

HelpButton::HelpButton(QWidget * parent)
:PixmapButton(parent)
{
    d = new Private;    
    
#ifdef Q_WS_MAC
    setPixmap(QPixmap(":/Icons/HelpButtonMac.png"));
    setPixmapHl(QPixmap(":/Icons/HelpButtonMac.png"));
    setPixmapDown(QPixmap(":/Icons/HelpButtonMacPressed.png"));
#else
    setPixmap(QPixmap(":/Icons/HelpButton.png"));
    setPixmapHl(QPixmap(":/Icons/HelpButton.png"));
    setPixmapDown(QPixmap(":/Icons/HelpButtonPressed.png"));
#endif
    
    connect(this, SIGNAL(clicked()), this, SLOT(openHelp()));
}

HelpButton::HelpButton(QString keyword, QWidget * parent)
:PixmapButton( parent )	
{
    d = new Private;
    setKeyword(keyword);
    
#ifdef Q_WS_MAC
    setPixmap(QPixmap(":/Icons/HelpButtonMac.png"));
    setPixmapHl(QPixmap(":/Icons/HelpButtonMac.png"));
    setPixmapDown(QPixmap(":/Icons/HelpButtonMacPressed.png"));
#else
    setPixmap(QPixmap(":/Icons/HelpButton.png"));
    setPixmapHl(QPixmap(":/Icons/HelpButton.png"));
    setPixmapDown(QPixmap(":/Icons/HelpButtonPressed.png"));
#endif

    connect(this, SIGNAL(clicked()), this, SLOT(openHelp()));
}

HelpButton::~HelpButton()
{
    delete d;
}

QString HelpButton::keyword() const
{
    return d->keyword;
}

void HelpButton::setKeyword(const QString & keyword)
{
    d->keyword = keyword;
}

void HelpButton::openHelp()
{
    if (keyword().isEmpty()) return;
    QString page = QString("contents/%1.html").arg(keyword());

    if (qApp->metaObject()->indexOfMethod("showDocumentationPage(QString)") != -1)
        qApp->metaObject()->invokeMethod(qApp, "showDocumentationPage", 
                                         Qt::DirectConnection,
                                         Q_ARG(QString,page));
    else qDebug() << "No slot: Application::showDocumentationPage(QString)";
}

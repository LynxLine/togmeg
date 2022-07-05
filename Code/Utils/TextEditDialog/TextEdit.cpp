//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#include <QtGui>
#ifdef QT_NETWORK_LIB
    #include <QtNetwork>
#endif
#include "TextEdit.h"
#include "MiscUtils.h"

class TextEdit::Private {
public:
    Private()
    :mode(Default)
    {}
    TextEdit::Mode mode;
    QTextCharFormat formatPainter;
    QString imagesDirPath;
    QHash<QString, QString> resourceForImage;
    
#ifdef QT_NETWORK_LIB
    QNetworkAccessManager *netManager;
#endif
};

TextEdit::TextEdit(QWidget * parent)
:QTextEdit(parent)
{
    d = new Private;
    
#ifdef QT_NETWORK_LIB
    d->netManager = new QNetworkAccessManager(this);
    connect(d->netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(imageDownloaded(QNetworkReply*)));
#endif
}

TextEdit::~TextEdit()
{
    delete d;
}

TextEdit::Mode TextEdit::mode()
{
    return d->mode;
}

void TextEdit::setImagesDirPath(QString path) {
    if (!path.endsWith("/")) path += "/";
    d->imagesDirPath = path;
}

void TextEdit::setMode(TextEdit::Mode m)
{
    if (d->mode == m) return;
    d->mode = m;
    
    if (m == Default) {
        setCurrentCharFormat( d->formatPainter );
        QTextCursor cursor = textCursor();
        cursor.clearSelection();
        setTextCursor(cursor);
    }
    else if (m == FormatPainter) {
        d->formatPainter = currentCharFormat();
    }
    
    emit modeChanged(m);
}

void TextEdit::mouseReleaseEvent(QMouseEvent * me)
{
    if (d->mode == FormatPainter) setMode(Default);
    QTextEdit::mouseReleaseEvent(me);
}

bool TextEdit::canInsertFromMimeData(const QMimeData * source) const
{
    if (source->hasImage())
        return true;
    else return QTextEdit::canInsertFromMimeData(source);
}

void TextEdit::insertFromMimeData(const QMimeData * source)
{
    QStringList info = source->formats();
    if (source->hasImage()) {
        QImage image = qvariant_cast<QImage>(source->imageData());
        QTextCursor cursor = this->textCursor();
        QTextDocument * document = this->document();
        QString imageUrl = MiscUtils::uniqueId() + ".png";
        document->addResource(QTextDocument::ImageResource, QUrl(imageUrl), image);
        cursor.insertImage(imageUrl);
        emit textChanged();
        return;
    }
    QTextEdit::insertFromMimeData(source);
    checkForExternalImages();
}

void TextEdit::checkForExternalImages() 
{
    QRegExp rx_local = QRegExp("^file://");
    QRegExp rx_url   = QRegExp("^http(s)?://");

    QList<QString> downloadStarted;

    QTextDocument *document = this->document();
    QList<QTextFragment> images = imageFragments();
    for(QList<QTextFragment>::iterator it=images.begin(); it!=images.end(); it++) {
        QTextFragment fragment = *it;
        QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
        QString imageUrl = imageFormat.name();

        QImage image = qvariant_cast<QImage>(document->resource(QTextDocument::ImageResource, QUrl(imageUrl)));

        // Skip image if resource already present
        if (!image.isNull()) continue; 
        
        if (d->resourceForImage.contains(imageUrl)) {
            // Already loaded image. Just rename it.
            fixImageFragment(fragment, d->resourceForImage[imageUrl]);
        } 
        else {
            if (rx_local.indexIn(imageUrl) != -1) {
                // Try to load image from local file.
                image.load(QUrl(imageUrl).toLocalFile());
                if (image.isNull()) continue;
                QString newImageUrl = MiscUtils::uniqueId() + ".png";
                d->resourceForImage[imageUrl] = newImageUrl;
                document->addResource(QTextDocument::ImageResource, QUrl(newImageUrl), image);
                fixImageFragment(fragment, newImageUrl);
            } 
            else if (rx_url.indexIn(imageUrl) != -1 && !downloadStarted.contains(imageUrl)) {
#ifdef QT_NETWORK_LIB
                // Try to download image.
                downloadStarted.push_back(imageUrl);
                d->netManager->get(QNetworkRequest(imageUrl));
#endif
            } 
        }
    }
    emit textChanged();
}

void TextEdit::fixImageFragment(QTextFragment fragment, QString newImageUrl) 
{
    QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
    imageFormat.setName(newImageUrl);
    
    QTextCursor cursor(this->document());
    cursor.setPosition(fragment.position());
    cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
    cursor.setCharFormat(imageFormat);
}

QVariant TextEdit::loadResource(int type, const QUrl & name) 
{
    if (type == QTextDocument::ImageResource)
        return QImage(d->imagesDirPath + name.toString());

    return QTextEdit::loadResource(type, name);
}

#ifdef QT_NETWORK_LIB
void TextEdit::imageDownloaded(QNetworkReply * reply) 
{
    QString imageUrl = reply->url().toString();
    QImageReader ir(reply);
    QImage image = ir.read();
    
    if (!image.isNull()) {
        QString newImageUrl = MiscUtils::uniqueId() + ".png";
        document()->addResource(QTextDocument::ImageResource, QUrl(newImageUrl), image);
        d->resourceForImage[imageUrl] = newImageUrl;

        foreach(QTextFragment f, imageFragments(imageUrl))
            fixImageFragment(f, newImageUrl);
    }
    reply->close();
    emit textChanged();
}
#endif

QList<QTextFragment> TextEdit::imageFragments(const QString & name) 
{
    QList<QTextFragment> list;
    QTextDocument * doc = this->document();

    bool checkName = !name.isEmpty();
    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next()) {
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); it++) {
            QTextFragment fragment = it.fragment();
            QTextCharFormat format = fragment.charFormat();
            if (!format.isImageFormat()) continue;
            if (checkName && name != format.toImageFormat().name()) continue;
            list.push_back(fragment);
        }
    }
    return list;
}

//
// TextDocument
//

class TextDocument::Private {
public:
    QString imagesDirPath;
};

TextDocument::TextDocument(QObject * parent)
:QTextDocument(parent)
{
    d = new Private;
}

TextDocument::~TextDocument()
{
    delete d;
}

void TextDocument::setImagesDirPath(QString path) {
    if (!path.endsWith("/")) path += "/";
    d->imagesDirPath = path;
}

QVariant TextDocument::loadResource(int type, const QUrl & name) 
{
    if (type == QTextDocument::ImageResource)
        return QImage(d->imagesDirPath + name.toString());

    return QTextDocument::loadResource(type, name);
}

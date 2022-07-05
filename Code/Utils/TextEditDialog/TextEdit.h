//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QTextFragment>

class QNetworkReply;

class TextEdit : public QTextEdit {
Q_OBJECT
public:
    enum Mode {
        Default =0,
        FormatPainter
    };
    
    TextEdit(QWidget * parent = 0L);
    virtual ~TextEdit();
    
    Mode mode();
    void setMode(Mode);

    QVariant loadResource ( int type, const QUrl & name );
    void setImagesDirPath (QString path);

    QList<QTextFragment> imageFragments(const QString & name = QString() );

signals:
    //! Emits on mode change
    void modeChanged(TextEdit::Mode);
    
protected:
    virtual void mouseReleaseEvent(QMouseEvent * me);
    virtual bool canInsertFromMimeData(const QMimeData *) const;
    virtual void insertFromMimeData(const QMimeData * source);
    
private slots:
#ifdef QT_NETWORK_LIB
    void imageDownloaded(QNetworkReply*);     
#endif
    
private:
    class Private;
    Private * d;   

    void checkForExternalImages();
    void fixImageFragment(QTextFragment fragment, QString newImageUrl);
};

class TextDocument : public QTextDocument {
    Q_OBJECT
public:
    TextDocument(QObject * parent = 0L);
    virtual ~TextDocument();
    
    void setImagesDirPath(QString path);
    virtual QVariant loadResource(int type, const QUrl & name);
    
private:
    class Private;
    Private * d;   
};

#endif // TEXTEDIT_H

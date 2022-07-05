//
// Copyright Petrostreamz 2008
//

#ifndef XML_Reader_H
#define XML_Reader_H

#include <QXmlDefaultHandler>
#include "XML_Handler.h"

class XML_Reader : public QXmlDefaultHandler
{
public:
    enum ErrorLevel { NOERROR, WARNING, ERROR, FATAL, NUMLEVELS };
    
    XML_Reader(XML_Handler * top, QString topTag, QFile &, QTextStream * log);
    XML_Reader(XML_Handler * top, QString topTag, QIODevice *source, QTextStream * log);
    virtual ~XML_Reader();
    
    ErrorLevel Read_XML();
    
    virtual bool startElement(const QString & namespaceURI,
                              const QString & localName, const QString & qName,
                              const QXmlAttributes & atts);
    virtual bool endElement(const QString & namespaceURI,
                            const QString & localName, const QString & qName);
    virtual bool characters(const QString & ch);
    
    bool error(const QXmlParseException & exception);
    bool warning(const QXmlParseException & exception);
    bool fatalError(const QXmlParseException & exception);    
    QString errorString() const;
    
private:    
    static const QString alert[NUMLEVELS];
    void ParseException(ErrorLevel level, const QXmlParseException & exception);
    
private:
    class Private;
    Private * d;
};

#endif // XML_Reader_H

//
// Copyright Petrostreamz 2008
//

#include <QtCore>
#include "XML_Reader.h"
#include "XML_Handler.h"

class XML_Reader::Private {
public:
    Private()
    :file(0L), 
     logStream(0L),
     errorLevel(NOERROR)
    {}
    
    QFile * file;
    QIODevice *source;
    QTextStream * logStream;
    XML_Handler * top;
    QString topTag;
    
    QStack<QString> tagStack;
    QStack<XML_Handler *> elStack;
    
    ErrorLevel errorLevel;
    QString	errorMessage;
};

const QString XML_Reader::alert[NUMLEVELS] = 
{
	QString("Alert"),
	QString("Warning"),
	QString("Error"),
	QString("Fatal error")
};

XML_Reader::XML_Reader(XML_Handler * top, QString topTag, QFile & file, QTextStream * log) 
:QXmlDefaultHandler()
{
    d = new Private;
    d->file = &file;
    d->source = d->file;
    d->top = top;
    d->topTag = topTag;
    d->logStream = log;
    d->errorLevel = NOERROR;    
}

XML_Reader::XML_Reader(XML_Handler * top, QString topTag, QIODevice *source, QTextStream * log) 
:QXmlDefaultHandler()
{
    d = new Private;
    d->source = source;
    d->top = top;
    d->topTag = topTag;
    d->logStream = log;
    d->errorLevel = NOERROR;    
}

XML_Reader::~XML_Reader()
{
    delete d;
}

XML_Reader::ErrorLevel XML_Reader::Read_XML()
{
    d->errorLevel = NOERROR;
    d->errorMessage.clear();
    d->tagStack.clear();

    QXmlInputSource xmlSource(d->source);
    QXmlSimpleReader xmlReader;
    xmlReader.setContentHandler(this);
    xmlReader.setErrorHandler(this);
    xmlReader.parse(&xmlSource);
    return d->errorLevel;
}

bool XML_Reader::startElement(const QString & namespaceURI,
                                  const QString & localName, 
                                  const QString & qName, 
                                  const QXmlAttributes & atts)
{
    Q_UNUSED(qName);
    Q_UNUSED(namespaceURI);

    bool ok = true;
    if ( !d->elStack.empty() ) {
        XML_Handler * h = d->elStack.top();
        h = h->Start_Tag(localName, d->errorMessage);
        if (h) h->Read_Attributes(localName, atts, d->errorMessage);
        
        d->elStack.push(h);
        d->tagStack.push(localName);
    }
    else if ( localName == d->topTag ) {
        d->top->Start_Tag(localName, d->errorMessage);
        d->top->Read_Attributes(localName, atts, d->errorMessage);
        d->tagStack.push(localName);
        d->elStack.push(d->top);
        ok = true;
    }
    else if ( d->file ){
        QString filePath = QFileInfo(*d->file).absoluteFilePath();
        d->errorMessage = QObject::tr("File \"%1\" is not a valid Linkz file.").arg(filePath);
        return false;
    }
    else {
        d->errorMessage = QObject::tr("Source XML is invalid.");
        return false;
    }
    return ok;
}

bool XML_Reader::endElement(const QString & namespaceURI,
                                const QString & localName, 
                                const QString & qName)
{
    Q_UNUSED(qName);
    Q_UNUSED(namespaceURI);
    
    if (d->elStack.empty())
        return false;
    
    d->tagStack.pop();
    XML_Handler * h = d->elStack.pop();
    if (!h) return false;
    
    bool ok = h->End_Tag(localName, d->errorMessage);
    return ok;
}

bool XML_Reader::characters(const QString & ch)
{
    if (d->elStack.empty())
        return false;
    
    XML_Handler * h = d->elStack.top();
    if (!h) return false;
    
    bool ok = h->Read_Text(d->tagStack.top(), ch, d->errorMessage);
    return ok;
}

bool XML_Reader::warning(const QXmlParseException & exception)
{
    ParseException(WARNING, exception);
    return true;
}

bool XML_Reader::error(const QXmlParseException & exception)
{
    ParseException(ERROR, exception);
    return true;
}

bool XML_Reader::fatalError(const QXmlParseException & exception)
{
    ParseException(FATAL, exception);
    return false;
}

QString XML_Reader::errorString() const
{
    return d->errorMessage;
}

void XML_Reader::ParseException(ErrorLevel level, const QXmlParseException & exception)
{
    if (!d->logStream) 
        return;

    QTextStream & log = *d->logStream;
    
    QFileInfo info;
    if ( d->file ) {
        info.setFile(*d->file);
    }
    QString message = exception.message();
    log << "\n" << alert[level] << ": " << message;
    if ( message.size() && !message[message.size()-1].isPunct() )
        log << ".";
    log << "\n";
    log << "Problem detected at line " << exception.lineNumber()
        << ", column " << exception.columnNumber();
    if (d->file) {
        log <<  ", of XML file \"" << info.absoluteFilePath()
            << "\"." << endl;
    } 
    else {
        log <<  "." << endl;
    }
    d->errorLevel = std::max(d->errorLevel,level);
}

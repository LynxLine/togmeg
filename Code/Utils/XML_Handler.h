//
// Copyright Petrostreamz 2009
//

#ifndef XML_Handler_H
#define XML_Handler_H

#include <QString>
#include "XML_Util.h"
class QXmlAttributes;
using namespace PetroStreamz;

class XML_Handler {
public:
    XML_Handler() {;}
    virtual ~XML_Handler() {;}
    
    virtual XML_Handler * Start_Tag(const QString & tag, QString & err) =0;
    
    virtual bool End_Tag(const QString & tag, QString & err) {
        Q_UNUSED(tag);
        Q_UNUSED(err);
        return true;
    }
    
    virtual bool Read_Attributes(const QString & tag, const QXmlAttributes &, QString & err) {
        Q_UNUSED(tag);
        Q_UNUSED(err);
        return true;
    }
    
    virtual bool Read_Text(const QString & tag, const QString & ch, QString & err) {
        Q_UNUSED(ch);
        Q_UNUSED(tag);
        Q_UNUSED(err);
        return true;
    }
    
};

class XML_Tag_Collector : public XML_Handler {
public:
    XML_Tag_Collector(QString _tag):tag(_tag) {;}
    virtual ~XML_Tag_Collector() {;}

    virtual XML_Handler * Start_Tag(const QString & tag, QString &) {
        children << XML_Tag_Collector(tag);
        return &children.back();
    }

    virtual bool Read_Attributes(const QString &, const QXmlAttributes & _atts, QString &) { atts = _atts; return true; }
    virtual bool Read_Text(const QString &, const QString & _ch, QString &) { ch = _ch; return true; }

    void Write_Data(QTextStream & out) const {
        if (children.empty() && ch.isEmpty()) {
            XML_Util::Write_Empty_Tag(out,tag,atts);
        }
        else if (children.empty()) {
            XML_Util::Write_Complete_Tag(out, tag, ch, atts);
        }
        else {
            XML_Util::Write_Start_Tag(out, tag, atts);
            foreach(const XML_Tag_Collector & child, children) child.Write_Data(out);
            XML_Util::Write_End_Tag(out, tag);
        }
    }

private:
    QString ch;
    QString tag;
    QXmlAttributes atts;
    QList<XML_Tag_Collector> children;
};

#endif // XML_Handler_H

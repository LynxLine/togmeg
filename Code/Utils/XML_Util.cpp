/*******************************************************************************
/	File XML_Util.cpp
*******************************************************************************/

#include "XML_Util.h"

namespace PetroStreamz {

/*******************************************************************************
/	XML Utility Definitions
*******************************************************************************/

void XML_Util::Write_XML_Preamble(QTextStream &out)
{
	out.setCodec("UTF-8");
	out.seek(0);
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\""
		<< " standalone=\"yes\"?>" << "\n";
}

/******************************************************************************/

QString XML_Util::XML_Ready(const QString &s)
{
	QString result = s;
	result.replace('&',"&amp;");
	result.replace('<',"&lt;");
	result.replace('>',"&gt;");
	result.replace('\'',"&apos;");
	result.replace('\"',"&quot;");
	return result;
}

/******************************************************************************/

void XML_Util::Append_Attribute(const QString &name, const QString &value,
	QXmlAttributes &atts)
{
	static const QString empty("");
	if ( !value.isEmpty() ) atts.append(name,empty,name,value);
}

/******************************************************************************/

void XML_Util::Write_Start_Tag(QTextStream &out, const QString &tag,
	const QXmlAttributes &atts, const QString &ending)
{
	QString spacer;
	if ( atts.count() > 1 ) {
		spacer.fill(' ',tag.size()+2);
		spacer.replace(0,1,'\n');
	}
	out << "<" << tag;
	for ( int i = 0; i < atts.count(); ) {
		out << " " << atts.localName(i) << "=\""
			<< XML_Ready(atts.value(i)) << "\"";
		if ( ++i < atts.count() ) out << spacer;
	}
	out << ending;
}

/******************************************************************************/

void XML_Util::Write_End_Tag(QTextStream &out, const QString &tag)
{
	out << "</" << tag << ">" << "\n";
}

/******************************************************************************/

} // namespace PetroStreamz

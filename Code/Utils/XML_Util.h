/*******************************************************************************
/	File XML_Util.h
*******************************************************************************/

#ifndef XML_UTIL_H
#define XML_UTIL_H

#include <QTextStream>
#include <QXmlAttributes>

namespace PetroStreamz {

//!\ingroup Runner
/*@{*/
    
/*******************************************************************************
/	XML Utility Class
*******************************************************************************/

class XML_Util
{
public:

	static void Write_XML_Preamble(QTextStream &out);

	static QString XML_Ready(const QString &s);

	static void Append_Attribute(const QString &name, const QString &value,
		QXmlAttributes &atts);

	static void Write_Start_Tag(QTextStream &out, const QString &tag,
		const QXmlAttributes &atts = QXmlAttributes(),
		const QString &ending = QString(">\n"));

	static void Write_End_Tag(QTextStream &out, const QString &tag);

	static void Write_Empty_Tag(QTextStream &out, const QString &tag,
		const QXmlAttributes &atts = QXmlAttributes())
	{
		Write_Start_Tag(out,tag,atts,QString("/>\n"));
	}

	template<typename T>
	static void Write_Complete_Tag(QTextStream &out, const QString &tag,
		const T &value, const QXmlAttributes &atts = QXmlAttributes())
	{
		Write_Start_Tag(out,tag,atts,QString(">"));
		out << value;
		Write_End_Tag(out,tag);
	}
    
};

/******************************************************************************/

/*@}*/
    
} // namespace PetroStreamz

#endif

//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "IconSet.h"


class IconSet::Private {
public:
    static QHash<QString, QIcon> iconMap;
    static QHash<QString, QPixmap> pixmapMap;
};

QHash<QString, QIcon> IconSet::Private::iconMap;
QHash<QString, QPixmap> IconSet::Private::pixmapMap;

/*!
 Return QIcon by name. It prepends ":/Icons/"
 and append ".png" if not provided. These icons
 are embedded into Pipe-It executable.
 */
QIcon IconSet::icon(QString name)
{
    if (name.isEmpty())
        return QIcon();
    
    if (!name.startsWith(":/Icons/")) name = ":/Icons/"+name;
    if (!name.endsWith(".png")) name += ".png";
    
    if (!Private::iconMap.contains(name))
        Private::iconMap[name] = QIcon(name);
        
    return Private::iconMap[name];
}

/*!
 Return QPixmap by name. It prepends ":/Icons/"
 and append ".png" if not provided. These pixmaps
 are embedded into Pipe-It executable.
 */
QPixmap IconSet::pixmap(QString name)
{
    if (name.isEmpty())
        return QPixmap();
    
    if (!name.endsWith(".png")) name += ".png";
    if (!name.startsWith(":/Icons/") && !name.startsWith(":/Pixmaps/")) {
        if (QFileInfo(":/Pixmaps/"+name).exists()) name = ":/Pixmaps/"+name;
        if (QFileInfo(":/Icons/"+name).exists()) name = ":/Icons/"+name;
    }    
    
    if (!Private::pixmapMap.contains(name))
        Private::pixmapMap[name] = QPixmap(name);
    
    return Private::pixmapMap[name];
}


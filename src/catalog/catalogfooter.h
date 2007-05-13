//
// Copyright (C) 2007 Oleksandr Yakovlyev <yshurik@gmail.com>
//

#ifndef CATALOGFOOTER_H
#define CATALOGFOOTER_H

#include <QWidget>

/*!
 * \class CatalogFooter
 */
class CatalogFooter : public QWidget
{
Q_OBJECT
public:
    CatalogFooter(QWidget * parent = 0);
    virtual ~CatalogFooter();

public slots:
    void setStudyCount(int);
    void setCurrentTask(QString taskId);

signals:
    void renameClicked();

private slots:
    void updateMessageLabel();

private:
    class Private;
    Private * d;
};

#include "pixmapbutton.h"

class RenamePixmapButton : public PixmapButton
{
Q_OBJECT
public:
    RenamePixmapButton(QWidget * parent = 0);

protected:
    virtual void paintEvent(QPaintEvent * pe);

};

#endif // CATALOGFOOTER_H

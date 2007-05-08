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

private slots:
    void updateMessageLabel();

private:
    class Private;
    Private * d;
};

#endif // CATALOGFOOTER_H

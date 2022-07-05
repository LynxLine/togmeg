//
// Copyright Petrostreamz 2007
//

#ifndef HELPBUTTON_H
#define HELPBUTTON_H

#include "PixmapButton.h"

/*!
 \class HelpButton
 */
class HelpButton : public PixmapButton
{
Q_OBJECT
public:
    HelpButton(QString keyword, QWidget * parent =0);
    HelpButton(QWidget * parent =0);
    ~HelpButton();
    
    QString keyword() const;
    void setKeyword(const QString &);

private slots:
    void openHelp();
    
private:
    class Private;
    Private * d;
};

#endif // HELPBUTTON_H

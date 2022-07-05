//
// Copyright Petrostreamz 2007
//

#ifndef FormTextEdit_H
#define FormTextEdit_H

#include <QDialog>
#include <QTextCharFormat>
#include <QTextListFormat>

#include "TextEdit.h"

class QTextBlockFormat;
        
/*!
 Text edit box for canvas anotations
 */
class FormTextEdit : public QDialog
{
Q_OBJECT
public:
    FormTextEdit(QString html, QWidget * parent);
    virtual ~FormTextEdit();

    QString htmlText() const;
    TextEdit * textEdit() const;

    void setDefaultFont(QFont);
    void setCurrentAlignment(Qt::Alignment);
    void initReferences(QStringList);

signals:
    void contentChanged();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

protected slots:
    virtual void accept();
    virtual void reject();
    virtual void setControlData();
    
    virtual void setAlignment(Qt::Alignment);
    virtual void applyData();
    
    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(QTextListFormat::Style);
    void setTextColor(const QColor &);
    void textSubScript();
    void textSuperScript();
    void textHAlign(QAction *a);
    void textVAlign(QAction *a);
    void unindentText();
    void indentText();
    void createList();
    
    void leftMarginChanged(const QString &m);
    void rightMarginChanged(const QString &m);
    void bottomMarginChanged(const QString &m);
    void topMarginChanged(const QString &m);
    void marginsChanged(); 
    
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();
    
    void clipboardDataChanged();
    
    void setFormatPainterMode(bool);
    void formaterPainterModeChanged(TextEdit::Mode);

    void insertReference(int);

private:
    void createToolBar();
    bool testDoubleClick(QAction *);
    
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void mergeBlockFormat(const QTextBlockFormat &format);

    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void styleChanged();
    void textScriptChanged(QTextCharFormat::VerticalAlignment);
    void alignmentChanged(Qt::Alignment a);
    
private:
    class Private;
    Private * d;

    static QRect s_rectGeometry;
};

#endif

/****************************************************************************
**
** Copyright (C) 2003-2007 Trolltech ASA. All rights reserved.
**
** This file is part of a Qt Solutions component.
**
** Licensees holding a valid Qt Solutions License Agreement may use this
** file in accordance with the rights, responsibilities, and obligations
** contained therein. Please consult your licensing agreement or contact
** sales@trolltech.com if any conditions of this licensing are not clear
** to you.
**
** Further information about Qt Solutions licensing is available at:
** http://www.trolltech.com/products/qt/addon/solutions/ 
** or by contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QTCOLORPICKER_H
#define QTCOLORPICKER_H
#include <QtGui/QToolButton>
#include <QtCore/QString>
#include <QtGui/QColor>

#include <QtGui/QLabel>
#include <QtCore/QEvent>
#include <QtGui/QFocusEvent>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTCOLORPICKER_EXPORT) && !defined(QT_QTCOLORPICKER_IMPORT)
#    define QT_QTCOLORPICKER_EXPORT
#  elif defined(QT_QTCOLORPICKER_IMPORT)
#    if defined(QT_QTCOLORPICKER_EXPORT)
#      undef QT_QTCOLORPICKER_EXPORT
#    endif
#    define QT_QTCOLORPICKER_EXPORT __declspec(dllimport)
#  elif defined(QT_QTCOLORPICKER_EXPORT)
#    undef QT_QTCOLORPICKER_EXPORT
#    define QT_QTCOLORPICKER_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTCOLORPICKER_EXPORT
#endif

class ColorPickerPopup;

class QT_QTCOLORPICKER_EXPORT QtColorPicker : public QToolButton
{
    Q_OBJECT

    Q_PROPERTY(bool colorDialog READ colorDialogEnabled WRITE setColorDialogEnabled)

public:
    QtColorPicker(QWidget *parent = 0,
                  int columns = -1, bool enableColorDialog = true);

    ~QtColorPicker();

    void insertColor(const QColor &color, const QString &text = QString::null, int index = -1);

    QColor currentColor() const;

    QColor color(int index) const;

    void setColorDialogEnabled(bool enabled);
    bool colorDialogEnabled() const;

    void setStandardColors();

    static QColor getColor(const QPoint &pos, bool allowCustomColors = true);

public Q_SLOTS:
    void setCurrentColor(const QColor &col);

Q_SIGNALS:
    void colorChanged(const QColor &);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private Q_SLOTS:
    void buttonPressed(bool toggled);
    void popupClosed();

private:
    ColorPickerPopup *popup;
    QColor col;
    bool withColorDialog;
    bool dirty;
    bool firstInserted;
};

#endif

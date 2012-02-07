/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2012
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
**/

#ifndef QX11GRAB_DRAWTEXT_H
#define QX11GRAB_DRAWTEXT_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class FontPreview;
class DropShadowBox;
class ColorPreview;
class TextPosition;

class Q_DECL_EXPORT drawtext : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    /** types for color dialog */
    enum ColorType { BACKGROUND, FOREGROUND, TEXTSHADOW };

    /** drawed Text previewer */
    FontPreview* m_fontPreview;

    /** groubox for shadow configuration */
    DropShadowBox* m_dropShadowBox;

    /** color preview label for drawed text */
    ColorPreview* m_foregroundPreview;

    /** color preview label for background */
    ColorPreview* m_backgroundPreview;

    /** set drawed text Position */
    TextPosition* m_textPosition;

    /** icon for combobox items */
    QIcon fontIcon;

    /** current selected font */
    QFont currentFont;

    /** absolute fontfile path */
    QString fontFilePath;

    /** font selector */
    QComboBox* m_fontComboBox;

    /** text to draw */
    QLineEdit* m_textInputEdit;

    /** slider for font size */
    QSlider* m_sliderSize;

    /** filter commandline */
    QLineEdit* m_lineEditOutput;

    /** freetype fontfile path */
    QString fileFilePath;

    /** text color */
    QColor fontcolor;

    /** preview background color */
    QColor boxcolor;

    /** text shadow color */
    QColor shadowcolor;

    /** get all .ttf files in fontconfig database */
    void initFontConfigDatabase();

    /** open qcolordialog and set type */
    void openColorChooser ( ColorType type );

  private Q_SLOTS:
    /** combobox items changed */
    void fontIndexChanged ( int );

    /** Slider Size Changed */
    void fontSizeChanged ( int );

    /** set previewer background color */
    void setBackgroundColor();

    /** set text color */
    void setForegroundColor();

    /** set shadow color */
    void setShadowColor();

    /** set shadow x, y offset */
    void setShadowOffset ( int );

    /** set shadow alpha */
    void setShadowAlpha ( int );

    /** update the commandline and all previewers */
    void updateFont();

    /** copy dataset into clipboard */
    void clipper();

  public:
    explicit drawtext ( QWidget * parent = 0 );

    /** initial fontconfig and start dialog */
    int start();

    /** filter commandline */
    Q_SCRIPTABLE const QString value();

    virtual ~drawtext();
};

#endif


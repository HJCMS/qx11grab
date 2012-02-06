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

class Q_DECL_EXPORT drawtext : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    enum ColorType { BACKGROUND, FOREGROUND, TEXTSHADOW };
    FontPreview* m_fontPreview;
    DropShadowBox* m_dropShadowBox;
    ColorPreview* m_foregroundPreview;
    ColorPreview* m_backgroundPreview;
    QIcon fontIcon;
    QFont currentFont;
    QString fontFilePath;
    QComboBox* m_fontComboBox;
    QLineEdit* m_lineEdit;
    QSlider* m_sliderSize;
    QLineEdit* m_lineEditOutput;
    QString fileFilePath;
    QColor fontcolor;
    QColor boxcolor;
    QColor shadowcolor;

    void initFontConfigDatabase();
    void openColorChooser ( ColorType type );

  private Q_SLOTS:
    void fontIndexChanged ( int );
    void fontSizeChanged ( int );
    void setBackgroundColor();
    void setForegroundColor();
    void setShadowColor();
    void setShadowOffset ( int );
    void setShadowAlpha ( int );
    void updateFont();

  public:
    explicit drawtext ( QWidget * parent = 0 );
    int start();
    Q_SCRIPTABLE const QString value();
    ~drawtext();
};

#endif


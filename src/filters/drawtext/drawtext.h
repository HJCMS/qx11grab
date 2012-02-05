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
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class FontBox;

class Q_DECL_EXPORT drawtext : public QDialog
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    enum ColorType { BACKGROUND, FOREGROUND, TEXTSHADOW };
    FontBox* m_fontBox;
    QLineEdit* m_lineEditOutput;
    QSpinBox* m_shadowOffset;
    QDoubleSpinBox* m_shadowAlpha;
    QString fileFilePath;
    QString fontcolor;
    QString boxcolor;
    QString shadowcolor;

    void openColorChooser ( ColorType type );

  private Q_SLOTS:
    void setBackgroundColor();
    void setForegroundColor();
    void setShadowColor();
    void updateFont();
    void updatePreview ( int );
    void updatePreview ( double );

  public:
    explicit drawtext ( QWidget * parent = 0 );
    int start();
    Q_SCRIPTABLE const QString value();
    ~drawtext();
};

#endif


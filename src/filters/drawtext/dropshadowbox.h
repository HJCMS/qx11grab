/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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
*/

#ifndef DROPSHADOWBOX_H
#define DROPSHADOWBOX_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QGroupBox>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

class ColorPreview;

class DropShadowBox : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    ColorPreview* m_colorPreview;
    QToolButton* m_buttonSetColor;
    QSlider* m_shadowOffset;
    QSlider* m_shadowAlpha;

  Q_SIGNALS:
    void buttonClicked();
    void offsetChanged ( int );
    void alphaChanged ( int );

  public Q_SLOTS:
    void setShadowColor ( const QColor &color );
    void setShadowOffset ( int );
    void setShadowAlpha ( int );

  public:
    DropShadowBox ( QWidget * parent = 0 );
    ~DropShadowBox();
};

#endif

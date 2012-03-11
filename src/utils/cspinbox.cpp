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

#include "cspinbox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QLineF>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionSpinBox>

/**
* Modifizierte SpinBox
* Ist der Eingabe Wert nicht Teilbar wird links ein Roter Seitenbalken gezeichnet!
* Diese Klasse wird Hauptsächlich für die Eingabe der Dimensionen verwendet.
*/
CSpinBox::CSpinBox ( QWidget * parent )
    : QSpinBox ( parent )
{
  setObjectName ( QLatin1String ( "CSpinBox" ) );
}

/**
* Zeichne bei Ungeraden Werten einen roten Seitenbalken!
*/
void CSpinBox::paintEvent ( QPaintEvent * event )
{
  QSpinBox::paintEvent ( event );
  if ( ( value() % 2 ) != 0 )
  {
    QStyleOptionSpinBox panel;
    initStyleOption ( &panel );
    QLineF lf ( panel.rect.topLeft(), panel.rect.bottomLeft() );
    QPainter painter ( this );
    painter.setPen ( Qt::red );
    painter.drawLine ( lf );
  }
}

CSpinBox::~CSpinBox()
{}

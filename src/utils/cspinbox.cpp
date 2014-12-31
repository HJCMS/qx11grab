/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#include "cspinbox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QLineF>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QPalette>
#include <QtGui/QStyleOptionSpinBox>

/**
* Modifizierte SpinBox
* Ist der Eingabe Wert nicht Teilbar wird links ein Roter Seitenbalken gezeichnet!
* Diese Klasse wird Hauptsächlich für die Eingabe der Dimensionen verwendet.
*/
CSpinBox::CSpinBox ( QWidget * parent )
    : QSpinBox ( parent )
    , invalid ( Qt::red )
    , normal ( palette().text().color() )
{
  setObjectName ( QLatin1String ( "CSpinBox" ) );
  setCorrectionMode ( QAbstractSpinBox::CorrectToPreviousValue );
}

/**
* Bei Ungeraden Werten den Text hervorheben!
*/
void CSpinBox::paintEvent ( QPaintEvent * event )
{
  QStyleOptionSpinBox option;
  option.initFrom ( this );
  QPalette p = option.palette;
  p.setColor ( QPalette::Text, ( ( value() & 1 ) ? invalid : normal ) );
  setPalette ( p );
  QSpinBox::paintEvent ( event );
}

/**
* Bei Ungeraden Werten \b QValidator::Invalid zurück geben!
*/
QValidator::State CSpinBox::validate ( QString &input, int &pos ) const
{
  Q_UNUSED ( pos );
  bool b;
  int i = input.toUInt ( &b, 10 );
  if ( b )
    return ( i & 1 ) ? QValidator::Invalid : QValidator::Acceptable;

  return QValidator::Acceptable; // fallback
}

CSpinBox::~CSpinBox()
{}

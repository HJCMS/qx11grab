/*
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
*/

#include "screenbox.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QLineEdit>

ScreenBox::ScreenBox ( QWidget * parent )
    : QDoubleSpinBox ( parent )
{
  setObjectName ( QLatin1String ( "ScreenBox" ) );
  setPrefix ( ":" );
  setDecimals ( 1 );
  lineEdit()->setInputMask ( ":0.0" );
  setRange ( 0.0, 9.0 );
  setSingleStep ( 1.0 );
  setValue ( 0.0 );
  setCorrectionMode ( QAbstractSpinBox::CorrectToPreviousValue );
  setButtonSymbols ( QAbstractSpinBox::PlusMinus );
}

void ScreenBox::stepBy ( int i )
{
  double s = ( lineEdit()->cursorPosition() <= 3 ) ? 1.0 : 0.1;
  setValue ( ( ( i == 1 ) ? ( value() + s ) : ( value() - s ) ) );
}

QAbstractSpinBox::StepEnabled ScreenBox::stepEnabled () const
{
  double v = value();
  if ( ( v >= 0.1 ) && ( v <= 9.0 ) )
    return ( QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled );
  else if ( v >= 0.0 )
    return QAbstractSpinBox::StepUpEnabled;
  else
    return QAbstractSpinBox::StepNone;
}

ScreenBox::~ScreenBox()
{}

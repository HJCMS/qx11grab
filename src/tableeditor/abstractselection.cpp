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

#include "abstractselection.h"

/* QtCore */
#include <QtCore/QDebug>

AbstractSelection::AbstractSelection ( QWidget * parent )
    : QComboBox ( parent )
{
  setEditable ( false );
}

void AbstractSelection::setValue ( const QVariant &value )
{
  int index = findData ( value, Qt::DisplayRole, ( Qt::MatchExactly | Qt::MatchCaseSensitive ) );
  if ( index != -1 )
    setCurrentIndex ( index );
}

const QVariant AbstractSelection::value()
{
  QVariant val;
  if ( itemData ( currentIndex(), Qt::UserRole ).toBool() )
    return itemData ( currentIndex(), Qt::DisplayRole );

  return val;
}

QByteArray AbstractSelection::valuePropertyName () const
{
  return QByteArray ( "value" );
}

AbstractSelection::~AbstractSelection()
{}

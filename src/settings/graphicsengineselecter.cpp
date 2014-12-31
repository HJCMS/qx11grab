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

#include "graphicsengineselecter.h"

/* QtCore */
#include <QtCore/QDebug>

GraphicsEngineSelecter::GraphicsEngineSelecter ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "GraphicsEngineSelecter" ) );
  /*: WhatsThis */
  setWhatsThis ( trUtf8 ( "Set default graphics backend to system" ) );
  insertItem ( 0, trUtf8 ( "Native" ), QString ( "native" ) );
  insertItem ( 0, trUtf8 ( "Raster" ), QString ( "raster" ) );
#ifdef HAVE_OPENGL
  insertItem ( 0, trUtf8 ( "OpenGL" ), QString ( "opengl" ) );
#endif
  setCurrentIndex ( 0 );
}

void GraphicsEngineSelecter::setValue ( const QString &value )
{
  for ( int i = 0; i < count(); ++i )
  {
    if ( itemData ( i, Qt::UserRole ).toString().compare ( value ) == 0 )
    {
      setCurrentIndex ( i );
      return;
    }
  }
  setCurrentIndex ( 0 );
}

const QString GraphicsEngineSelecter::value()
{
  return itemData ( currentIndex(), Qt::UserRole ).toString();
}

GraphicsEngineSelecter::~GraphicsEngineSelecter()
{}

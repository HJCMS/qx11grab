/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#include "settings.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

Settings::Settings ( QObject *parent )
    : QSettings ( QSettings::NativeFormat, QSettings::UserScope, "hjcms.de", "qx11grab", parent )
{
  /** Veraltete Einstellungen bei einem neuen Versions Start in die Tonne drücken. */
  if ( ! value ( QLatin1String ( "Version" ) ).toString().contains ( QX11GRAB_VERSION ) )
  {
    // DEPRECATED Config Options
    remove ( "windowPos" );
    remove ( "windowSize" );
    remove ( "windowState" );
    remove ( "enable_pulse_pasuspender" );
    remove ( "metadata" ); // new style
  }
  else
    setValue ( QLatin1String ( "Version" ), QX11GRAB_VERSION );
}

const QHash<QString,QVariant> Settings::readGroup ( const QString &group )
{
  QHash <QString,QVariant> map;
  int size = beginReadArray ( group );
  if ( size < 1 )
  {
    endArray(); // Nicht vergessen ;)
    return map;
  }

  for ( int i = 0; i < size; i++ )
  {
    setArrayIndex ( i );
    map[ value ( "argument" ).toString() ] =  value ( "value", "" );
  }
  endArray();

  return map;
}

Settings::~Settings()
{}

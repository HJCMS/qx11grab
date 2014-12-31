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

#include "setdarplugin.h"
#include "setdar.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

bool SetDarPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_setdar = new SetDar ( parent );
    if ( m_setdar )
      return true;
  }
  m_setdar = 0;
  return false;
}

bool SetDarPlugin::exec()
{
  return ( m_setdar->exec() == QDialog::Accepted ) ? true : false;
}

const QString SetDarPlugin::pluginName()
{
  return QString::fromUtf8 ( "setdar" );
}

const QString SetDarPlugin::title()
{
  return trUtf8 ( "Display Aspect Ratio" );
}

const QString SetDarPlugin::description()
{
  return trUtf8 ( "Set the Display Aspect Ratio for the filter output video." );
}

const QString SetDarPlugin::data()
{
  return m_setdar->data();
}

Q_EXPORT_PLUGIN2 ( SetDar, SetDarPlugin )


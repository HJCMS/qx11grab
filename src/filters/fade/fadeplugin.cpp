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

#include "fadeplugin.h"
#include "fade.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

bool fadePlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_fade = new fade ( parent );
    if ( m_fade )
      return true;
  }
  m_fade = 0;
  return false;
}

bool fadePlugin::exec()
{
  return ( m_fade->start() == QDialog::Accepted ) ? true : false;
}

const QString fadePlugin::pluginName()
{
  return QString::fromUtf8 ( "fade" );
}

const QString fadePlugin::title()
{
  return trUtf8 ( "Fade Filter" );
}

const QString fadePlugin::description()
{
  return trUtf8 ( "Fade (in/out) Filter" );
}

const QString fadePlugin::data()
{
  QString val = m_fade->data();
  if ( val.isEmpty() )
    return QString::fromUtf8 ( "fade=in:0:10" ); // 10 Frames

  return val;
}

Q_EXPORT_PLUGIN2 ( fade, fadePlugin )


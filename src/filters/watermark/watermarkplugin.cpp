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

#include "watermarkplugin.h"
#include "watermark.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

bool watermarkPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_watermark = new Watermark ( parent );
    if ( m_watermark )
      return true;
  }
  m_watermark = 0;
  return false;
}

bool watermarkPlugin::exec()
{
  return ( m_watermark->start() == QDialog::Accepted ) ? true : false;
}

const QString watermarkPlugin::pluginName()
{
  return QString::fromUtf8 ( "watermark" );
}

const QString watermarkPlugin::title()
{
  return trUtf8 ( "Watermark" );
}

const QString watermarkPlugin::description()
{
  return trUtf8 ( "Insert a Watermark into output video" );
}

const QString watermarkPlugin::data()
{
  QString val = m_watermark->data();
  if ( ! val.contains ( "movie=" ) )
    return QString();

  return val;
}

Q_EXPORT_PLUGIN2 ( watermark, watermarkPlugin )


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

#include "webcamplugin.h"
#include "webcam.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

bool webcamPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_webcam = new Webcam ( parent );
    if ( m_webcam )
      return true;
  }
  m_webcam = 0;
  return false;
}

bool webcamPlugin::exec()
{
  return ( m_webcam->start() == QDialog::Accepted ) ? true : false;
}

const QString webcamPlugin::pluginName()
{
  return QString::fromUtf8 ( "webcam" );
}

const QString webcamPlugin::title()
{
  return trUtf8 ( "Webcam" );
}

const QString webcamPlugin::description()
{
  return trUtf8 ( "Insert a Webcam into output video" );
}

const QString webcamPlugin::data()
{
  QString val = m_webcam->data();
  if ( ! val.contains ( "movie=" ) )
    return QString();

  return val;
}

Q_EXPORT_PLUGIN2 ( webcam, webcamPlugin )

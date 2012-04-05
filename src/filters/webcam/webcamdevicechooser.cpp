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

#include "webcamdevicechooser.h"
#include "webcamdeviceinfo.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QIcon>

/* QX11Grab */
#include "v4l2-api.h"

WebCamDeviceChooser::WebCamDeviceChooser ( QWidget * parent )
    : QComboBox ( parent )
    , cameraIcon ( QIcon::fromTheme ( "camera-web" ) )
{
  setObjectName ( QLatin1String ( "WebCamDeviceChooser" ) );
}

const QString WebCamDeviceChooser::toString ( unsigned char* ptr ) const
{
  return  QString::fromUtf8 ( reinterpret_cast<char *> ( ptr ) );
}

bool WebCamDeviceChooser::isWebCamDevice ( const QFileInfo &dev )
{
  bool status = false;
  if ( ! dev.isReadable() )
    return status;

  v4l2* p_v4l2 = new v4l2;
  if ( p_v4l2->open ( dev.absoluteFilePath() , false ) )
  {
    struct v4l2_capability m_cap;
    if ( p_v4l2->querycap ( m_cap ) )
    {
      WebCamDeviceInfo devInfo ( dev.absoluteFilePath(),
                                 toString ( m_cap.driver ),
                                 toString ( m_cap.card ),
                                 toString ( m_cap.bus_info )
                               );

      qDebug() << Q_FUNC_INFO << devInfo.path() << devInfo.driver() << devInfo.card() << devInfo.bus();
    }
    p_v4l2->close();
    status = true;
  }
  delete p_v4l2;
  return status;
}

void WebCamDeviceChooser::searchDevices()
{
  QDir dir ( QLatin1String ( "/dev" ), QLatin1String ( "video*" ), QDir::Name, QDir::System );
  foreach ( QFileInfo info, dir.entryInfoList() )
  {
    if ( isWebCamDevice ( info ) )
      addItem ( cameraIcon, info.baseName(), info.absoluteFilePath() );
  }

}

void WebCamDeviceChooser::init()
{
  clear();
  searchDevices();
}

WebCamDeviceChooser::~WebCamDeviceChooser()
{}

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

/* Kernel */
#include <linux/videodev2.h>

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
  addItem ( cameraIcon, trUtf8 ( "No web camera found" ), false );
  setEnabled ( false );
}

const QString WebCamDeviceChooser::toString ( unsigned char* ptr ) const
{
  const char* data = reinterpret_cast<char *> ( ptr );
  return  QString::fromAscii ( data );
}

bool WebCamDeviceChooser::isWebCamDevice ( const QFileInfo &dev )
{
  bool status = false;
  if ( ! dev.isReadable() )
    return status;

  v4l2 p_v4l2;
  if ( p_v4l2.open ( dev.absoluteFilePath() , false ) )
  {
    // Setze erst mal auf true und suche nach TV Karte
    // wenn gefunden dann auf false zurück setzen!
    status = true;
    v4l2_input input;
    if ( p_v4l2.enum_input ( input, true ) )
    {
      do
      {
        if ( ( input.capabilities & V4L2_IN_CAP_STD )
               || ( input.capabilities & V4L2_IN_CAP_PRESETS ) )
        {
          status = false;
          break;
        }
      }
      while ( p_v4l2.enum_input ( input ) );
    }

    // Ist es keine TV Karte dann Informationen sammeln und in die Liste einfügen
    if ( status )
    {
      struct v4l2_capability m_cap;
      if ( p_v4l2.querycap ( m_cap ) )
      {
        WebCamDeviceInfo info;
        info.path = dev.absoluteFilePath();
        info.driver = toString ( m_cap.driver );
        info.card = toString ( m_cap.card );
        info.bus = toString ( m_cap.bus_info );
        devInfo.append ( info );
      }
      else
        status = false; // wenn zugriffs fehler dann zurück setzen
    }
    // alle infos vollständig dann wieder schliessen
    p_v4l2.close();
  }

  return status;
}

void WebCamDeviceChooser::searchDevices()
{
  QDir dir ( QLatin1String ( "/dev" ), QLatin1String ( "video*" ), QDir::Name, QDir::System );
  QFileInfoList items = dir.entryInfoList();
  if ( items.size() > 0 )
  {
    foreach ( QFileInfo info, items )
    {
      if ( ! isWebCamDevice ( info ) )
        continue;
    }

    if ( devInfo.size() > 0 )
    {
      setEnabled ( true );
      clear();
      addItem ( cameraIcon, trUtf8 ( "Choose Camera" ), false );

      qRegisterMetaType<WebCamDeviceInfo>();

      for ( int i = 0 ; i < devInfo.size(); ++i )
      {
        WebCamDeviceInfo info = devInfo.at ( i );

        QVariant data;
        data.setValue ( info );

        QString title = QString::fromUtf8 ( "%1 (%2)" ).arg ( info.card, info.driver );
        addItem ( cameraIcon, title, data );
      }
    }
  }
}

void WebCamDeviceChooser::init()
{
  devInfo.clear();
  searchDevices();
}

WebCamDeviceChooser::~WebCamDeviceChooser()
{}

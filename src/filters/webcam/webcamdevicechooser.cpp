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

#include "webcamdevicechooser.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QRect>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>

/* V4L2 */
#include <linux/videodev2.h>
#include "v4l2-api.h"

WebCamDeviceChooser::WebCamDeviceChooser ( QWidget * parent )
    : QWidget ( parent )
    , cameraIcon ( QIcon::fromTheme ( "camera-web" ) )
{
  setObjectName ( QLatin1String ( "WebCamDeviceChooser" ) );

  QHBoxLayout* layout = new QHBoxLayout ( this );

  m_setWebcamDevice = new QComboBox ( this );
  m_setWebcamDevice->addItem ( cameraIcon, trUtf8 ( "No web camera found" ), false );
  m_setWebcamDevice->setEnabled ( false );
  layout->addWidget ( m_setWebcamDevice );

  m_setCaptureSize = new QComboBox ( this );
  m_setCaptureSize->setToolTip ( trUtf8 ( "capture from framesize" ) );
  m_setCaptureSize->setWhatsThis ( trUtf8 ( "set maximum input framesize to capture from source" ) );
  QIcon captureIcon = QIcon::fromTheme ( "media-record" );
  m_setCaptureSize->addItem ( captureIcon, QLatin1String ( "160x120" ), QSize ( 160, 120 ) );
  m_setCaptureSize->addItem ( captureIcon, QLatin1String ( "320x240" ), QSize ( 320, 240 ) );
  m_setCaptureSize->addItem ( captureIcon, QLatin1String ( "640x480" ), QSize ( 640, 480 ) );
  layout->addWidget ( m_setCaptureSize );

  setLayout ( layout );

  connect ( m_setWebcamDevice, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( itemChanged ( int ) ) );

  connect ( m_setCaptureSize, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( itemSizeChanged ( int ) ) );
}

/**
* Schnittstellen Einträge in QString umwandeln
*/
const QString WebCamDeviceChooser::toString ( unsigned char* ptr ) const
{
  const char* data = reinterpret_cast<const char*> ( ptr );
  return  QString::fromAscii ( data );
}

/**
* Öffne mit dem übergebenen Dateipfad die Schnittstelle und
* sehe nach ob es ich um eine Kamera handelt. Ist dies der Fall
* dann setze einen Eintrag in \ref devInfo und gebe true zurück.
*/
bool WebCamDeviceChooser::insertWebCamDevice ( const QFileInfo &dev )
{
  bool status = false;
  WebCamDeviceInfo info;

  if ( ! dev.isReadable() )
    return status;

  v4l2 p_v4l2;
  if ( p_v4l2.open ( dev.absoluteFilePath() , false ) )
  {
    // Zuerst sehen wir nach ob es ich um eine Kamera handelt
    v4l2_input input;
    if ( p_v4l2.enum_input ( input, true ) )
    {
      do
      {
        if ( ( input.type & V4L2_INPUT_TYPE_CAMERA ) )
        {
          // qDebug() << Q_FUNC_INFO << toString( input.name );
          status = true;
          break;
        }
      }
      while ( p_v4l2.enum_input ( input ) );
    }

    // Ist es eine Kamera dann Informationen sammeln und in die Liste einfügen
    if ( status )
    {
      struct v4l2_capability m_cap;
      if ( p_v4l2.querycap ( m_cap ) )
      {
        info.path = dev.absoluteFilePath();
        info.driver = toString ( m_cap.driver );
        info.card = toString ( m_cap.card );
        info.bus = toString ( m_cap.bus_info );
      }
      else
        status = false; // wenn zugriffs fehler dann zurück setzen
    }

    // Suche nach den Luminance+Chrominance formaten
    if ( status )
    {
      v4l2_fmtdesc fdesc;
      v4l2_format fmt;
      if ( p_v4l2.enum_fmt_cap ( fdesc, true ) )
      {
        p_v4l2.g_fmt_cap ( fmt );
        do
        {
          info.pixfmt = p_v4l2.pixfmt2s ( fdesc.pixelformat );
          info.sourceSize = QSize ( fmt.fmt.pix.width, fmt.fmt.pix.height );
          info.outputSize = m_setCaptureSize->itemData ( m_setCaptureSize->currentIndex(), Qt::UserRole ).toSize();
          // qDebug() << Q_FUNC_INFO << info.size;
          break;
        }
        while ( p_v4l2.enum_fmt_cap ( fdesc ) );
      }
      else
      {
        // Wenn nicht gefunden Versuche YUV420 planar (V4L2_PIX_FMT_YUV420M)
        info.pixfmt = QLatin1String ( "yuv420p" );
        info.sourceSize = m_setCaptureSize->itemData ( m_setCaptureSize->currentIndex(), Qt::UserRole ).toSize();
        info.outputSize = m_setCaptureSize->itemData ( m_setCaptureSize->currentIndex(), Qt::UserRole ).toSize();
      }
    }

    // abfrage vollständig dann wieder schliessen
    p_v4l2.close();
  } // handle

  if ( status )
    devInfo.append ( info );

  return status;
}

/**
* Suche im Verzeichnis /dev nach video* Schnittstellen.
* Werden Schnittstellen gefunden sende für jeden Eintrag eine
* Anfrage an \ref insertWebCamDevice und nehme danach alle in
* \ref devInfo eingefügten Einträge für die ComboBox.
*/
void WebCamDeviceChooser::searchDevices()
{
  QDir dir ( QLatin1String ( "/dev" ), QLatin1String ( "video*" ), QDir::Name, QDir::System );
  QFileInfoList items = dir.entryInfoList();
  if ( items.size() > 0 )
  {
    foreach ( QFileInfo info, items )
    {
      if ( ! insertWebCamDevice ( info ) )
        continue;
    }

    if ( devInfo.size() > 0 )
    {
      m_setWebcamDevice->setEnabled ( true );
      m_setWebcamDevice->clear();
      int index = 0;
      m_setWebcamDevice->insertItem ( index, cameraIcon, trUtf8 ( "Choose Camera" ), false );

      qRegisterMetaType<WebCamDeviceInfo>();

      for ( int i = 0 ; i < devInfo.size(); ++i )
      {
        WebCamDeviceInfo info = devInfo.at ( i );

        QVariant data;
        data.setValue ( info );

        QString title = QString::fromUtf8 ( "%1 (%2 - %3)" )
                        .arg ( info.card, info.driver, info.pixfmt );

        m_setWebcamDevice->insertItem ( ++index, cameraIcon, title, data );
        m_setWebcamDevice->setItemData ( index, info.path, Qt::ToolTipRole );
      }
    }
  }
}

/**
* Ein Video Schnittstellen eintrag wurde ausgewählt
*/
void WebCamDeviceChooser::itemChanged ( int index )
{
  if ( index > 0 )
  {
    QVariant var = m_setWebcamDevice->itemData ( index, Qt::UserRole );
    WebCamDeviceInfo info = var.value<WebCamDeviceInfo>();
    emit cameraSelected ( info );
  }
}

/**
* Die Video Einstellungen für die Aufnahme Größe wurde geändert
*/
void WebCamDeviceChooser::itemSizeChanged ( int index )
{
  QSize size = m_setCaptureSize->itemData ( index, Qt::UserRole ).toSize();
  if ( size.isValid() )
    emit captureSizeSelected ( size );
}

/**
* Initialisiere alle Schnittstellen und befülle die ComboBox
*/
void WebCamDeviceChooser::init()
{
  devInfo.clear();
  searchDevices();
}

WebCamDeviceChooser::~WebCamDeviceChooser()
{}

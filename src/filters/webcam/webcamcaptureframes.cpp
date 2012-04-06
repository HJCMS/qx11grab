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

#include "webcamcaptureframes.h"

#include <errno.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>

WebCamCaptureFrames::WebCamCaptureFrames ( QWidget * parent )
    : QWidget ( parent )
    , m_v4l2 ( new v4l2 )
    , m_frameImage ( 0 )
    , m_socketNotifier ( 0 )
    , m_convertData ( 0 )
    , p_device ( QString() )
{
  setObjectName ( QLatin1String ( "WebCamCaptureFrames" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  m_timer = new QTimer ( this );

  m_button = new QPushButton ( trUtf8 ( "Capture" ), this );
  m_button->setIcon ( QIcon::fromTheme ( "media-record" ) );
  m_button->setEnabled ( false );
  layout->addWidget ( m_button );

  setLayout ( layout );

  connect ( m_button, SIGNAL ( clicked() ),
            this, SLOT ( buttonClicked() ) );

  connect ( m_timer, SIGNAL ( timeout() ),
            this, SLOT ( stopCapture() ) );
}

void WebCamCaptureFrames::startCaptureFrames ( bool b )
{
  if ( b )
  {
    // lese eingabe Format
    m_v4l2->g_fmt_cap ( m_inputFormat );
    // setze ausgabe format
    m_v4l2->s_fmt ( m_inputFormat );

    // Schreibe den Puffer
    m_streamData = new unsigned char[m_inputFormat.fmt.pix.sizeimage];
    m_outputFormat = m_inputFormat;
    m_outputFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

    v4lconvert_try_format ( m_convertData, &m_outputFormat, &m_inputFormat );
    m_v4l2->g_fmt_cap ( m_inputFormat );

    // Erstelle ein leeres Bild
    m_frameImage = new QImage ( m_outputFormat.fmt.pix.width, m_outputFormat.fmt.pix.height, QImage::Format_RGB888 );
    m_frameImage->fill ( 0 );

    m_socketNotifier = new QSocketNotifier ( m_v4l2->fd(), QSocketNotifier::Read, this );
    connect ( m_socketNotifier, SIGNAL ( activated ( int ) ), this, SLOT ( captureFrame ( int ) ) );
  }
}

void WebCamCaptureFrames::captureFrame ( int )
{
  int s = m_v4l2->read ( m_streamData, m_inputFormat.fmt.pix.sizeimage );
  if ( s < 0 )
  {
    if ( errno != EAGAIN )
    {
      qWarning ( "QX11Grab - webcam read error" );
      stopCapture();
    }
    return;
  }
  memcpy ( m_frameImage->bits(), m_streamData, s );

  int err = v4lconvert_convert ( m_convertData, &m_inputFormat, &m_outputFormat, m_streamData, s,
                                 m_frameImage->bits(), m_outputFormat.fmt.pix.sizeimage );

  if ( err == -1 )
  {
    qWarning ( "QX11Grab - webcam read error" );
    stopCapture();
    return;
  }

  if ( ! m_frameImage->isNull() )
    emit frameCaptered ( *m_frameImage );
}

void WebCamCaptureFrames::buttonClicked()
{
  if ( m_v4l2->fd() != -1 )
  {
    stopCapture();
  }
  else if ( m_v4l2->open ( p_device, false ) )
  {
    m_convertData = v4lconvert_create ( m_v4l2->fd(), NULL, &libv4l2_default_dev_ops );
    m_button->setText ( trUtf8 ( "Stop" ) );
    startCaptureFrames ( true );
    m_timer->start ( ( 1000 * 3 ) );
  }
}

void WebCamCaptureFrames::stopCapture()
{
  if ( m_socketNotifier )
  {
    m_socketNotifier->setEnabled ( false );
    delete m_socketNotifier;
    m_socketNotifier = 0;
  }

  v4l2_encoder_cmd cmd;
  memset ( &cmd, 0, sizeof ( cmd ) );
  cmd.cmd = V4L2_ENC_CMD_STOP;
  m_v4l2->ioctl ( VIDIOC_ENCODER_CMD, &cmd );

  if ( m_v4l2->fd() )
  {
    qDebug ( "QX11Grab - close capture device" );
    m_v4l2->close();
  }

  m_button->setText ( trUtf8 ( "Capture" ) );
  qDebug ( "QX11Grab - capture frames done" );
}

void WebCamCaptureFrames::setInterface ( const WebCamDeviceInfo &dev )
{
  QFileInfo info ( dev.path );
  if ( info.isReadable() )
  {
    p_device = dev.path;
    m_button->setEnabled ( true );
  }
  else
    m_button->setEnabled ( false );
}

WebCamCaptureFrames::~WebCamCaptureFrames()
{
  stopCapture();
}

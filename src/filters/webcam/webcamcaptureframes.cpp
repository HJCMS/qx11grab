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

#include "webcamcaptureframes.h"

/* std */
#include <errno.h>

/* Kernel/v4l2 */
#include <libv4lconvert.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QHBoxLayout>

WebCamCaptureFrames::WebCamCaptureFrames ( QWidget * parent )
    : QWidget ( parent )
    , m_v4l2 ( new v4l2 )
    , m_devInfo ( 0 )
    , m_frameImage ( 0 )
    , m_socketNotifier ( 0 )
    , m_convertData ( 0 )
{
  setObjectName ( QLatin1String ( "WebCamCaptureFrames" ) );
  setContentsMargins ( 0, 0, 0, 0 );

  QHBoxLayout* layout = new QHBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  m_timer = new QTimer ( this );

  m_button = new QPushButton ( trUtf8 ( "to capture" ), this );
  m_button->setIcon ( QIcon::fromTheme ( "media-record" ) );
  m_button->setEnabled ( false );
  layout->addWidget ( m_button );

  m_setTimout = new QSpinBox ( this );
  /*: ToolTip */
  m_setTimout->setToolTip ( trUtf8 ( "Timeout in seconds" ) );
  m_setTimout->setRange ( 3, 30 ); // Max 30 Sekunden
  m_setTimout->setValue ( 3 );
  layout->addWidget ( m_setTimout, Qt::AlignLeft );

  setLayout ( layout );

  connect ( m_button, SIGNAL ( clicked() ),
            this, SLOT ( buttonClicked() ) );

  connect ( m_timer, SIGNAL ( timeout() ),
            this, SLOT ( stopCapture() ) );
}

/**
* Belege den Datenstrom Speicher, bei Erfolg Starte
* mit QSocketNotifier das abgreifen des Puffers.
*/
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
    // Kopiere zum Konvertieren in den Ausgabe Puffer
    m_outputFormat = m_inputFormat;
    // Setze auf "24  RGB-8-8-8" siehe linux/videodev2.h
    m_outputFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

    // QImage bietet kein "YUV420 planar" an deshalb müssen wir immer konvertieren
    v4lconvert_try_format ( m_convertData,
                            &m_outputFormat,
                            &m_inputFormat );

    m_v4l2->g_fmt_cap ( m_inputFormat );

    // Erstelle ein leeres Bild
    qDebug() << "QX11Grab - Capture fromSize:" << m_outputFormat.fmt.pix.width << m_outputFormat.fmt.pix.height;
    m_frameImage = new QImage ( m_outputFormat.fmt.pix.width, m_outputFormat.fmt.pix.height, QImage::Format_RGB888 );
    m_frameImage->fill ( 0 );

    // Es wird immmer nur einen gewisse Zeit aufgenommen!
    m_timer->start ( ( 1000 * m_setTimout->value() ) );

    m_socketNotifier = new QSocketNotifier ( m_v4l2->fd(), QSocketNotifier::Read, this );
    connect ( m_socketNotifier, SIGNAL ( activated ( int ) ), this, SLOT ( captureFrame ( int ) ) );
  }
}

/**
* Versucht Bilder aus dem Datenstrom zu holen...
*/
void WebCamCaptureFrames::captureFrame ( int )
{
  int s = m_v4l2->read ( m_streamData, m_inputFormat.fmt.pix.sizeimage );
  if ( s < 0 )
  {
    // Bei schwerwiegenden Fehlern aussteigen
    if ( errno != EAGAIN )
    {
      qWarning ( "QX11Grab - webcam ioctl read error" );
      stopCapture();
    }
    // bei einem leeren puffer aussteigen
    return;
  }
  memcpy ( m_frameImage->bits(), m_streamData, s );

  int err = v4lconvert_convert ( m_convertData,
                                 &m_inputFormat,
                                 &m_outputFormat,
                                 m_streamData,
                                 s,
                                 m_frameImage->bits(),
                                 m_outputFormat.fmt.pix.sizeimage );

  if ( err == -1 )
  {
    qWarning ( "QX11Grab - webcam frame convert error" );
    stopCapture();
    return;
  }

  if ( ! m_frameImage->isNull() )
    emit frameCaptered ( *m_frameImage );
}

/**
* Ein/Abschalten der Aufnahme
*/
void WebCamCaptureFrames::buttonClicked()
{
  if ( m_v4l2->fd() >= 0 )
  {
    // laufende aufnahmen beenden
    stopCapture();
  }
  else if ( m_v4l2->open ( m_devInfo->path, false ) )
  {
    // QImage bietet kein "YUV420 planar" an deshalb müssen wir immer konvertieren
    // m_convertData = v4lconvert_create ( m_v4l2->fd(), NULL, &libv4l2_default_dev_ops );
    qDebug() << "QX11Grab - v4lconvert_create - " << m_devInfo->driver;
    m_convertData = v4lconvert_create ( m_v4l2->fd() );
    if ( ! m_convertData )
    {
      qWarning ( "QX11Grab - can not open device - %s",
                 v4lconvert_get_error_message ( m_convertData ) );
      return;
    }
    m_button->setText ( trUtf8 ( "Stop" ) );
    startCaptureFrames ( true );
  }
}

/**
* Bereinigen und Sauber beenden
* \warning Der Timer muß an dieser Stelle gestoppt werden.
*          Damit v4lconvert keinen Speicherzugriffsfehler erzeugt!
*/
void WebCamCaptureFrames::stopCapture()
{
  m_timer->stop();
  m_button->setText ( trUtf8 ( "to capture" ) );

  if ( m_socketNotifier )
  {
    m_socketNotifier->setEnabled ( false );
    delete m_socketNotifier;
    m_socketNotifier = 0;
  }

  if ( m_frameImage )
  {
    delete m_frameImage;
    m_frameImage = 0;
  }

  if ( m_v4l2->fd() >= 0 )
  {
    qDebug ( "QX11Grab - send v4l2 cmd stop" );
    v4l2_encoder_cmd cmd;
    memset ( &cmd, 0, sizeof ( cmd ) );
    cmd.cmd = V4L2_ENC_CMD_STOP;
    m_v4l2->ioctl ( VIDIOC_ENCODER_CMD, &cmd );

    qDebug ( "QX11Grab - restore stream buffer" );
    delete m_streamData;
    m_streamData = NULL;

    qDebug ( "QX11Grab - v4lconvert destroy buffer" );
    v4lconvert_destroy ( m_convertData );

    qDebug ( "QX11Grab - close capture device" );
    m_v4l2->close();

    qDebug ( "QX11Grab - capture frames done" );
  }
}

void WebCamCaptureFrames::setInterface ( const WebCamDeviceInfo &devInfo )
{
  QFileInfo info ( devInfo.path );
  m_button->setEnabled ( false );

  // Vorhandene Aufnahmen zuerst stoppen
  stopCapture();

  if ( info.isReadable() )
  {
    if ( m_devInfo )
    {
      qDebug ( "QX11Grab - destroy device info" );
      delete m_devInfo;
    }
    m_devInfo = new WebCamDeviceInfo ( devInfo );
    m_button->setEnabled ( true );
  }
}

const QSize WebCamCaptureFrames::captureSize()
{
  if ( m_devInfo )
    return m_devInfo->outputSize;

  return QSize ( 160, 120 );
}

WebCamCaptureFrames::~WebCamCaptureFrames()
{
  stopCapture();
}

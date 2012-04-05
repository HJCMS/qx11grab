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

#include "webcamdevice.h"

/* libv4l2 */
#include <libv4l2.h>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

WebCamDevice::WebCamDevice ( const QString &device, QObject * parent )
    : QThread ( parent )
    , v4l2()
    , v4l2dev ( device )
    , m_captureImage ( 0 )
    , m_listener ( 0 )
{
  setObjectName ( QLatin1String ( "WebCamDevice" ) );

  connect ( this,SIGNAL ( started() ),
            this, SLOT ( deviceOpened() ) );

  connect ( this,SIGNAL ( terminated() ),
            this, SLOT ( deviceClosed() ) );

  connect ( this,SIGNAL ( finished() ),
            this, SLOT ( deviceClosed() ) );
}

bool WebCamDevice::startCapture ()
{
  v4l2_requestbuffers req;
  unsigned int i;

  memset ( &req, 0, sizeof ( req ) );
  if ( ! reqbufs_mmap_cap ( req, 3 ) )
  {
    qWarning ( "QX11Grab - Cannot capture webcam" );
    return false;
  }
  else if ( req.count < 2 )
  {
    qWarning ( "QX11Grab - Too few buffers" );
    reqbufs_mmap_cap ( req );
    return false;
  }

  m_buffers = static_cast<buffer*> ( calloc ( req.count, sizeof ( *m_buffers ) ) );
  if ( ! m_buffers )
  {
    qWarning ( "QX11Grab - Out of memory" );
    reqbufs_mmap_cap ( req );
    return false;
  }

  for ( m_nbuffers = 0; m_nbuffers < req.count; ++m_nbuffers )
  {
    v4l2_buffer buf;

    memset ( &buf, 0, sizeof ( buf ) );

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = m_nbuffers;

    if ( -1 == ioctl ( VIDIOC_QUERYBUF, &buf ) )
    {
      perror ( "VIDIOC_QUERYBUF" );
      goto error;
    }

    m_buffers[m_nbuffers].length = buf.length;
    m_buffers[m_nbuffers].start = mmap ( buf.length, buf.m.offset );

    if ( MAP_FAILED == m_buffers[m_nbuffers].start )
    {
      perror ( "mmap" );
      goto error;
    }
  }

  for ( i = 0; i < m_nbuffers; ++i )
  {
    if ( !qbuf_mmap_cap ( i ) )
    {
      perror ( "VIDIOC_QBUF" );
      goto error;
    }
  }

  if ( !streamon_cap() )
  {
    perror ( "VIDIOC_STREAMON" );
    goto error;
  }
  return true;

error:
  return false;
}

void WebCamDevice::stopCapture()
{
  v4l2_requestbuffers reqbufs;
  unsigned i;

  delete m_captureImage;
  m_captureImage = 0;

  delete m_listener;
  m_listener = 0;

  if ( m_buffers == NULL )
    return;

  if ( ! streamoff_cap() )
    perror ( "VIDIOC_STREAMOFF" );

  for ( i = 0; i < m_nbuffers; ++i )
  {
    if ( -1 == munmap ( m_buffers[i].start, m_buffers[i].length ) )
      perror ( "munmap" );
  }

  // Free all buffers.
  reqbufs_mmap_cap ( reqbufs, 1 );

  // videobuf workaround
  reqbufs_mmap_cap ( reqbufs, 0 );
}

void WebCamDevice::captureFrame()
{
  v4l2_buffer buf;
  bool again;

  if ( ! dqbuf_mmap_cap ( buf, again ) )
  {
    qWarning ( "QX11Grab - dqbuf" );
    return;
  }

  if ( again )
    return;

  qbuf ( buf );

  if ( m_captureImage )
    emit frameEntered ( *m_captureImage );
}

void WebCamDevice::deviceOpened()
{
  v4l2_fmtdesc fmt;
  if ( enum_fmt_cap ( fmt, true ) )
  {
    do
    {
      qDebug() << pixfmt2s ( fmt.pixelformat );
    }
    while ( enum_fmt_cap ( fmt ) );
  }
  return;
//   static const struct
//   {
//     __u32 v4l2_pixfmt;
//     QImage::Format qt_pixfmt;
//   } supported_fmts[] =
//   {
//     { V4L2_PIX_FMT_RGB32, QImage::Format_ARGB32 },
//     { V4L2_PIX_FMT_RGB24, QImage::Format_RGB888 },
//     { V4L2_PIX_FMT_RGB565X, QImage::Format_RGB16 },
//     { V4L2_PIX_FMT_RGB555X, QImage::Format_RGB555 },
//     { V4L2_PIX_FMT_RGB444, QImage::Format_RGB444 },
//     { 0, QImage::Format_Invalid }
//   };
  QImage::Format dstFmt = QImage::Format_RGB888;

  bool convert = true;
  g_fmt_cap ( m_capSrcFormat );
  s_fmt ( m_capSrcFormat );

  m_frameData = new unsigned char[m_capSrcFormat.fmt.pix.sizeimage];
  m_capDestFormat = m_capSrcFormat;
  m_capDestFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

//   for ( int i = 0; supported_fmts[i].v4l2_pixfmt; i++ )
//   {
//     if ( supported_fmts[i].v4l2_pixfmt == m_capSrcFormat.fmt.pix.pixelformat )
//     {
//       m_capDestFormat.fmt.pix.pixelformat = supported_fmts[i].v4l2_pixfmt;
//       dstFmt = supported_fmts[i].qt_pixfmt;
//       convert = false;
//       break;
//     }
//   }
//
//   if ( convert )
//   {
//     v4lconvert_try_format ( m_convertData, &m_capDestFormat, &m_capSrcFormat );
//     g_fmt_cap ( m_capSrcFormat );
//   }

  m_captureImage = new QImage ( m_capDestFormat.fmt.pix.width, m_capDestFormat.fmt.pix.height, dstFmt );
  m_captureImage->fill ( 0 );

  if ( startCapture () )
  {
    m_listener = new QSocketNotifier ( fd(), QSocketNotifier::Read, this );
    connect ( m_listener, SIGNAL ( activated ( int ) ), this, SLOT ( captureFrame() ) );
    m_listener->setEnabled ( true );
  }
}

void WebCamDevice::deviceClosed()
{
  qDebug() << Q_FUNC_INFO;
}

void WebCamDevice::run()
{
  QFileInfo info ( v4l2dev );
  if ( info.isReadable() )
  {
    if ( ! open ( v4l2dev, true ) )
      return;

    m_convertData = v4lconvert_create ( fd(), NULL, &libv4l2_default_dev_ops );

    exec();

    v4lconvert_destroy ( m_convertData );
    v4l2::close();
  }
}

WebCamDevice::~WebCamDevice()
{}

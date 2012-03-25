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

#include "x11device.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>


#define OPT_GRAB   0x0040

namespace QX11Grab
{
  // Static globals for encoder, not accessed by more than one thread
  static pthread_mutex_t avcodec_mutex = PTHREAD_MUTEX_INITIALIZER;

  static AVFormatContext* avformatEncoder = NULL;
  static AVPacket avpacketEncoder;
  static bool isEncoderHeaderWritten = false;
  typedef struct
  {
    const char *name;
    int flags;
    union
    {
      void *dst_ptr;
      int ( *func_arg ) ( const char *, const char * );
      int ( *func2_arg ) ( void *, const char *, const char * );
      size_t off;
    } u;
    const char *help;
    const char *argname;
  } OptionDef;

  X11Device::X11Device ( QObject * parent )
      : QThread ( parent )
      , m_data ( new unsigned char() )
  {
    setObjectName ( QLatin1String ( "X11Device" ) );
    setPriority ( QThread::HighestPriority );

    AVCodec* buffer = NULL;

    pthread_mutex_lock ( &avcodec_mutex );
    av_register_all();
    libavcodec = avcodec_alloc_context3 ( buffer );

    AVCodec* coder = avcodec_find_encoder ( CODEC_ID_RAWVIDEO );
    avcodec_get_context_defaults3 ( libavcodec, coder );
    pthread_mutex_unlock ( &avcodec_mutex );

    m_data = static_cast<unsigned char*> ( av_mallocz ( 144000 ) );
  }

  void X11Device::run()
  {}

  X11Device::~X11Device()
  {
    pthread_mutex_lock ( &avcodec_mutex );
    avcodec_close ( libavcodec );
    av_free ( libavcodec );
    pthread_mutex_unlock ( &avcodec_mutex );

    av_free ( m_data );
  }
} /* eof namespace QX11Grab */

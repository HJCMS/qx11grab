/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#include "avoptions.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
// #include <QtGui>

/* FFmpeg */
extern "C"
{
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/samplefmt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/avstring.h>
#include <libavutil/opt.h>
}

namespace QX11Options
{

  AVOptions::AVOptions ( QObject * parent )
      : QObject ( parent )
  {
    setObjectName ( "AVOptions" );
  }

  const QList<FFOption> AVOptions::sampleFormats() const
  {
    QList<FFOption> list;
    char fmt_str[128];
    for ( int i = -1; i < AV_SAMPLE_FMT_NB; i++ )
    {
      av_get_sample_fmt_string ( fmt_str, sizeof ( fmt_str ), static_cast<AVSampleFormat> ( i ) );
      if ( i != -1 )
      {
        QStringList name = QString::fromUtf8 ( fmt_str ).split ( " " );
        FFOption opt;
        opt.id = i;
        opt.name = name.first();
        opt.value = QVariant();
        list.append ( opt );
      }
    }
    return list;
  }

  const QList<FFOption> AVOptions::pixelFormats() const
  {
    QList<FFOption> list;
    for ( int i = 0; i < PIX_FMT_NB; i++ )
    {
      const AVPixFmtDescriptor* pix_desc = &av_pix_fmt_descriptors[ static_cast<PixelFormat> ( i ) ];
      if ( ! pix_desc->name )
        continue;

      FFOption opt;
      opt.id = i;
      opt.name = QString::fromUtf8 ( pix_desc->name );
      opt.value = QVariant();
      list.append ( opt );
    }
    return list;
  }

  void AVOptions::getVideoCodecOption ( const QString &option, const QVariant &value )
  {
      qDebug() << Q_FUNC_INFO << "TODO" << option << value;
  }

  const QList<FFCodec> AVOptions::videoCodecs()
  {
    QList<FFCodec> list;
    av_register_all();
    AVCodec* codec;
    for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
    {
      if ( ( codec->type == AVMEDIA_TYPE_VIDEO ) && ( codec->encode ) )
      {
        FFCodec c;
        c.name = QString ( codec->name );
        c.fullname = QString ( codec->long_name );
        list.append ( c );
      }
    }
    return list;
  }

  const QList<FFCodec> AVOptions::audioCodecs()
  {
    QList<FFCodec> list;
    av_register_all();
    AVCodec* codec;
    for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
    {
      if ( ( codec->type == AVMEDIA_TYPE_AUDIO ) && ( codec->encode ) )
      {
        FFCodec c;
        c.name = QString ( codec->name );
        c.fullname = QString ( codec->long_name );
        list.append ( c );
      }
    }
    return list;
  }

  AVOptions::~AVOptions()
  {}

}  /* eof namespace QX11Options */

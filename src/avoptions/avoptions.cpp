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

#include <cstdlib>
#include <cstdio>

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

  const QList<FFOption> AVOptions::optionQuery ( const QByteArray &option )
  {
    QList<FFOption> output;
    AVCodecContext* avcodec_opts[AVMEDIA_TYPE_NB];
    const char *opt = option.constData();

    avcodec_register_all();
    av_register_all();

    // Initial Options
    for ( int i = 0; i < AVMEDIA_TYPE_NB; i++ )
    {
      avcodec_opts[i] = avcodec_alloc_context2 ( static_cast<AVMediaType> ( i ) );
    }

    qDebug ( "Option query:%s", opt );
    for ( int type = 0; ( *avcodec_opts && ( type < AVMEDIA_TYPE_NB ) ); type++ )
    {
      const AVOption *o2 = av_opt_find ( avcodec_opts[0], opt, NULL, AV_OPT_FLAG_VIDEO_PARAM, 0 );
      if ( o2 )
      {
        FFOption opt;
        qDebug() << "VIDEO:" << o2->name << ":" << o2->help << ":" << o2->default_val.str;
        opt.name = QString ( o2->name );
        opt.value = QString ( o2->default_val.str );
        opt.help = QString ( o2->help );
        output.append ( opt );
        break;
      }
      else if ( ( o2 = av_opt_find ( avcodec_opts[0], opt, NULL, AV_OPT_FLAG_AUDIO_PARAM, 0 ) ) )
      {
        FFOption opt;
        qDebug() << "AUDIO:" << o2->name << ":" << o2->help << ":" << o2->default_val.str;
        opt.name = QString ( o2->name );
        opt.value = QString ( o2->default_val.str );
        opt.help = QString ( o2->help );
        output.append ( opt );
        break;
      }
    }

    // UnRegister Options
    for ( int i = 0; i < AVMEDIA_TYPE_NB; i++ )
    {
      av_freep ( &avcodec_opts[i] );
    }

    return output;
  }

  const QList<FFOption> AVOptions::aspect()
  {
    QList<FFOption> list;
    av_register_all();
    FFOption opt;

    opt.id = 0;
    opt.name = "4:3";
    opt.value = QVariant ( "1.3333" );
    list.append ( opt );

    opt.id = 1;
    opt.name = "16:9";
    opt.value = QVariant ( "1.7777" );
    list.append ( opt );
    return list;
  }

  const QList<FFOption> AVOptions::meMethod()
  {
    QStringList items ( "epzs" );
    items << "full" << "zero" << "esa" << "tesa" << "dia";
    items << "log" << "phods" << "x1" << "hex" << "umh" << "iter";

    QList<FFOption> list;
    av_register_all();
    for ( int i = 0; i < items.size(); ++i )
    {
      FFOption opt;
      opt.id = i;
      opt.name = items.at ( i );
      opt.value = QVariant ();
      list.append ( opt );
    }
    return list;
  }

  const QList<FFOption> AVOptions::sampleFormats()
  {
    QList<FFOption> list;
    av_register_all();
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

  const QList<FFOption> AVOptions::pixelFormats()
  {
    QList<FFOption> list;
    av_register_all();
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

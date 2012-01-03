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
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/samplefmt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/avstring.h>
#include <libavutil/opt.h>
}

namespace QX11Grab
{
  /** generate basic ffmpeg options
  * @ref http://ffmpeg.org/doxygen/
  */
  AVOptions::AVOptions ( QObject * parent )
      : QObject ( parent )
  {
    setObjectName ( "AVOptions" );
  }

  /**
  * Liest Standard Kodierer Einträge in AVCodecContext
  * und sendet das Signal codecDefaults
  */
  void AVOptions::initCodecDefaults ( CodecID codecId )
  {
    if ( codecId == CODEC_ID_NONE )
      return;

    AVCodec* buffer = NULL;
    AVCodec* codec = avcodec_find_encoder ( codecId );
    if ( ! codec )
      return;

    AVCodecContext* avc = avcodec_alloc_context3 ( buffer );
    /* Do not call this function if a non-NULL codec has been passed to
    * avcodec_alloc_context3() that allocated this AVCodecContext. */
    int ret = avcodec_get_context_defaults3 ( avc, codec );
    if ( ret == 0 )
      emit codecDefaults ( avc );
  }

  /**
  * Suche für die Typen (AV_OPT_FLAG_VIDEO_PARAM|AV_OPT_FLAG_AUDIO_PARAM)
  * nach Optionen und ihrer Hilfe Texte.
  * \todo Standard Werte Vorschläge!
  */
  const QList<FFOption> AVOptions::optionQuery ( const QByteArray &option )
  {
    QList<FFOption> output;
    FFOption ffOption;
    AVCodecContext* avcodec_opts[AVMEDIA_TYPE_NB];
    const char *opt = option.constData();

    avcodec_register_all();

    // initial options buffer
    for ( int i = 0; i < AVMEDIA_TYPE_NB; i++ )
    {
      avcodec_opts[i] = avcodec_alloc_context2 ( static_cast<AVMediaType> ( i ) );
    }

    qDebug ( "Searching for: %s", opt );
    const AVOption *obj = av_opt_find ( avcodec_opts[0], opt, NULL, AV_OPT_FLAG_VIDEO_PARAM, 0 );
    if ( obj )
    {
      qDebug ( "Found video option for Name:%s Help:%s", obj->name, obj->help );
      ffOption.name = QString ( obj->name );
      // if ( obj->type == FF_OPT_TYPE_STRING ) {}
      ffOption.value = QVariant();
      ffOption.help = QString ( obj->help );
      output.append ( ffOption );
    }
    else if ( ( obj = av_opt_find ( avcodec_opts[0], opt, NULL, AV_OPT_FLAG_AUDIO_PARAM, 0 ) ) )
    {
      qDebug ( "Found audio option for Name:%s Help:%s", obj->name, obj->help );
      ffOption.name = QString ( obj->name );
      // opt.value = QString ( obj->default_val.str );
      ffOption.value = QVariant();
      ffOption.help = QString ( obj->help );
      output.append ( ffOption );
    }

    // destroy options buffer
    for ( int i = 0; i < AVMEDIA_TYPE_NB; i++ )
    {
      av_freep ( &avcodec_opts[i] );
    }

    return output;
  }

  /**
  * Suche nach allen Audio Abfrage Formaten
  */
  const QList<FFOption> AVOptions::sampleFormats()
  {
    QList<FFOption> list;
    avcodec_register_all();
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
        opt.value = name.first();
        /*: ToolTip */
        opt.help = trUtf8 ( "sample format" );
        list.append ( opt );
      }
    }
    return list;
  }

  /**
  * Suche nach allen Video Pixel Formaten
  */
  const QList<FFOption> AVOptions::pixelFormats()
  {
    QList<FFOption> list;
    avcodec_register_all();
    for ( int i = 0; i < PIX_FMT_NB; i++ )
    {
      const AVPixFmtDescriptor* pix_desc = &av_pix_fmt_descriptors[ static_cast<PixelFormat> ( i ) ];
      if ( ! pix_desc->name )
        continue;

      FFOption opt;
      opt.id = i;
      opt.name = QString::fromUtf8 ( pix_desc->name );
      opt.value = QVariant ( pix_desc->name );
      /*: ToolTip */
      opt.help = trUtf8 ( "Bits per Pixel %1" )
                 .arg ( QString::number ( av_get_bits_per_pixel ( pix_desc ) ) );
      list.append ( opt );
    }
    return list;
  }

  /**
  * Eine Liste der verfügbaren Video Kodierer ausgeben
  */
  const QList<FFCodec> AVOptions::videoCodecs()
  {
    QList<FFCodec> list;
    avcodec_register_all();
    AVCodec* codec = NULL;
    for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
    {
      if ( ( codec->type == AVMEDIA_TYPE_VIDEO ) && ( codec->encode )
              && ! ( codec->capabilities & CODEC_CAP_EXPERIMENTAL ) )
      {
        // qDebug ( "VCodec: %s Capability: 0x%02x", codec->name, codec->capabilities );
        FFCodec c;
        c.id = codec->id;
        c.name = QString ( codec->name );
        c.fullname = QString ( codec->long_name );
        list.append ( c );
      }
    }
    return list;
  }

  /**
  * Eine Liste der verfügbaren Audio Kodierer ausgeben
  */
  const QList<FFCodec> AVOptions::audioCodecs()
  {
    QList<FFCodec> list;
    avcodec_register_all();
    AVCodec* codec = NULL;
    for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
    {
      if ( ( codec->type == AVMEDIA_TYPE_AUDIO ) && ( codec->encode )
              && ! ( codec->capabilities & CODEC_CAP_EXPERIMENTAL ) )
      {
        // qDebug ( "ACodec: %s ID: %d", codec->name, codec->id );
        FFCodec c;
        c.id = codec->id;
        c.name = QString ( codec->name );
        c.fullname = QString ( codec->long_name );
        list.append ( c );
      }
    }
    return list;
  }

  AVOptions::~AVOptions()
  {}

}  /* eof namespace QX11Grab */

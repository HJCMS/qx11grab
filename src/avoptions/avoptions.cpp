/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sched.h>

#include "avoptions.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

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

  /**
  * Prüfe ob dieser Codec nur zum kodieren geeignet ist.
  */
  static bool qx11grab_is_encoder ( AVCodec* _c )
  {
#if ( LIBAVCODEC_VERSION_MAJOR >= 53 )
    // qDebug() << "NEW_ENCODER_API" << LIBAVCODEC_VERSION_INT;
#if ( LIBAVCODEC_VERSION_MAJOR >= 54 )
    return ( ( _c && ( _c->encode2 ) ) == 1 );
#else
    return ( ( _c && ( _c->encode || _c->encode2 ) ) == 1 );
#endif
#else
    return ( ( _c && ( _c->encode ) ) == 1 );
#endif
  }

  /**
  * Wir können bei X11 nicht jeden Kodierer verwenden.
  * Aus diesem grund minimiere die Liste der Möglichkeiten
  * mit Hilfe eines Regulären Ausdrucks.
  * Liste der Aktuell gesuchten Kodierer:
  *   \li Alle lib\w+ Formate
  *   \li Alle dv Formate
  *   \li Alle mpeg Formate
  *   \li Alle h\d+ Formate
  *   \li Flash Format
  *   \li RAW und NUT
  * TODO Das hier ist im Moment nur eine Notlösung :-/
  */
  static bool qx11grab_supported_avcodec ( const char* name )
  {
    QString n = QString::fromUtf8 ( name );
    return n.contains ( QRegExp ( "((^lib\\w+)|(dv)|(mpeg)|(h\\d+)|(flv)|(raw)|(nut))", Qt::CaseInsensitive ) );
  }

  /** generate basic ffmpeg options
  * @ref http://ffmpeg.org/doxygen/
  */
  AVOptions::AVOptions ( QObject * parent )
      : QObject ( parent )
  {
    setObjectName ( "AVOptions" );
  }

  /**
  * Suche für die Typen (AV_OPT_FLAG_VIDEO_PARAM|AV_OPT_FLAG_AUDIO_PARAM)
  * nach Optionen und ihrer Hilfe Texte.
  * \todo Standard Werte Vorschläge!
  */
  const QList<FFOption> AVOptions::optionQuery ( const QByteArray &_option, const QByteArray &_codec )
  {
    QList<FFOption> output;
    FFOption ffOption;
    const char *opt = _option.constData();
    const char *data = _codec.data();

#ifdef MAINTAINER_REPOSITORY
    qDebug ( "AVOptions::optionQuery( Option: %s, Codec: %s )", opt, data );
#endif

    avcodec_register_all();
    // initial options buffer
    AVCodecContext* avcodec_opts = avcodec_alloc_context3 ( avcodec_find_decoder_by_name ( data ) );

    const AVOption *obj = av_opt_find ( avcodec_opts, opt, NULL, AV_OPT_FLAG_VIDEO_PARAM, AV_OPT_SEARCH_FAKE_OBJ );
    if ( obj )
    {
#ifdef MAINTAINER_REPOSITORY
      qDebug ( "Found video option for Name: %s Help: %s Default: %s Unit: %s",
               obj->name, obj->help, obj->default_val.str, obj->unit  );
#endif
      ffOption.name = QString ( obj->name );
      ffOption.value = QVariant();
      ffOption.help = QString ( obj->help );
      output.append ( ffOption );
      goto ready;
    }

    if ( ( obj = av_opt_find ( avcodec_opts, opt, NULL, AV_OPT_FLAG_AUDIO_PARAM, AV_OPT_SEARCH_FAKE_OBJ ) ) )
    {
#ifdef MAINTAINER_REPOSITORY
      qDebug ( "Found audio option for Name: %s Help: %s Default: %s Unit: %s",
               obj->name, obj->help, obj->default_val.str, obj->unit );
#endif
      ffOption.name = QString ( obj->name );
      ffOption.value = QVariant();
      ffOption.help = QString ( obj->help );
      output.append ( ffOption );
      goto ready;
    }

  ready:
    // destroy options buffer
    av_freep ( &avcodec_opts );

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
    AVCodec* m_codec = NULL;
    for ( m_codec = av_codec_next ( NULL ); m_codec != NULL; m_codec = av_codec_next ( m_codec ) )
    {
      if ( ! m_codec )
        continue;

      if ( ( m_codec->type == AVMEDIA_TYPE_VIDEO ) && qx11grab_is_encoder ( m_codec )
              && qx11grab_supported_avcodec ( m_codec->name ) )
      {
#ifdef MAINTAINER_REPOSITORY
        qDebug ( "VCodec %d: %s, ID: %d, Capability: 0x%02x",
                 qx11grab_supported_avcodec ( m_codec->name ),
                 m_codec->name,
                 m_codec->id,
                 m_codec->capabilities
               );
#endif
        // Wir verwenden auschließlich Kodierer die auch ein Ausgabe Format anbieten!
        if ( supportedFormats ( m_codec->id ).size() < 1 )
          continue;

        FFCodec c;
        c.id = m_codec->id;
        c.name = QString ( m_codec->name );
        c.fullname = QString ( m_codec->long_name );
        c.info = QString();
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
    AVCodec* m_codec = NULL;
    for ( m_codec = av_codec_next ( NULL ); m_codec != NULL; m_codec = av_codec_next ( m_codec ) )
    {
      if ( ! m_codec )
        continue;

      if ( ( m_codec->type == AVMEDIA_TYPE_AUDIO ) && qx11grab_is_encoder ( m_codec ) )
      {
        // qDebug ( "ACodec: %s ID: %d", m_codec->name, m_codec->id );
        FFCodec c;
        c.id = m_codec->id;
        c.name = QString ( m_codec->name );
        c.fullname = QString ( m_codec->long_name );
        c.info = QString();
        list.append ( c );
      }
    }
    return list;
  }

  /**
  * Erstellt eine Liste der verfügbaren Kodierer Erweiterungen
  * inklusive ihrer Formate. Wird auch von \ref videoCodecs verwendet
  * um fest zu stellen ob ein Ausgabe Format zu verfügung steht!
  */
  const QList<QX11Grab::FFFormat> AVOptions::supportedFormats ( AVCodecID id )
  {
    QList<QX11Grab::FFFormat> list;
    // alle codecs und demuxer initialisieren
    av_register_all();

    // ausgabe format
    AVOutputFormat* ofmt = NULL;

    // wir benötigen für den ersten eintrag einen puffer
    const char* buffer = "000";

    // starte endlos schleife
    forever
    {
      // setze format namen zurück
      const char* name = NULL;

      // setze den zeiger für AVOutputFormat
      while ( ( ofmt = av_oformat_next ( ofmt ) ) )
      {
        /* Ist der Formatname nicht leer und wurde noch
        * nicht gesetzt und ist es ein Video Kodierer ? */
        if ( ( name == NULL || strcmp ( ofmt->name, name ) < 0 )
                && ( strcmp ( ofmt->name, buffer ) > 0 )
                && ( ofmt->video_codec != AV_CODEC_ID_NONE ) )
        {
          /* Jetzt nachsehen ob dieser Kodierer die gleiche ID besitzt!
          * NOTE Das funktioniert erst jetzt (Warum ? keine Ahnung :-/) */
          if ( ofmt->video_codec == id )
          {
            // Keine Erweiterungen verfügbar, dann keine Verwendung dafür.
            QString extensions ( ofmt->extensions );
            if ( extensions.isEmpty() )
              continue;

            FFFormat f;
            f.format = QString ( ofmt->name );
            f.description = QString ( ofmt->long_name );
            f.defaultExt = QString ( ofmt->name );
            f.extensions = extensions.split ( "," );
            list.append ( f );
          }
        }
      }
      if ( name == NULL )
        break;

      buffer = name;
    }
    return list;
  }

  /** Eine Liste der Verfügbaren Benutzer Presets für diesen Codec */
  const QStringList AVOptions::userPresets ( const QString &suffix )
  {
    QStringList list;
    QDir d ( QDir::home() );

    // FFmpeg Benutzer Verzeichnisse
    QStringList targets ( ".ffmpeg" );
    targets << ".avconv";

    // filter für das preset Suchmuster
    QStringList nameFilters ( "*.ffpreset" );
    nameFilters << "*.avpreset";

    // Starte die Verzeichnissuche
    for ( int i = 0; i < targets.size(); ++i )
    {
      d.setPath ( QString::fromUtf8 ( "%1/%2" ).arg ( d.homePath(), targets.at ( i ) ) );
      if ( d.exists() )
      {
        foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
        {
          QString bn = info.completeBaseName();
          QRegExp pattern ( "^"+suffix+"\\-" );
          if ( bn.contains ( pattern ) )
          {
            list.append ( bn.replace ( pattern, "" ) );
          }
        }
      }
    }
    // Doppelte Einträge bei Symbolischen Verknüpfungen vermeiden!
    list.removeDuplicates();

    return list;
  }

  /** Eine Liste der Verfügbaren System Presets für diesen Codec */
  const QStringList AVOptions::systemPresets ( const QString &suffix )
  {
    QStringList list;
    QDir d ( QDir::home() );
    // Liste von System Verzeichnissen die durchsucht wird, starte mit /usr/share/ffmpeg
    QStringList targets ( "/usr/share/ffmpeg" );
    // suche AVCONV_DATADIR
    QByteArray buf = qgetenv ( "AVCONV_DATADIR" );
    if ( ! buf.isEmpty() )
      targets << QString::fromUtf8 ( buf );

    buf.clear(); // FFMPEG_DATADIR
    buf = qgetenv ( "FFMPEG_DATADIR" );
    if ( ! buf.isEmpty() )
      targets << QString::fromUtf8 ( buf );

    buf.clear();

    // filter für das preset Suchmuster
    QStringList nameFilters ( "*.ffpreset" );
    nameFilters << "*.avpreset";

    // Doppelte Verzeichnis Angaben entfernen
    targets.removeDuplicates();

    // Starte die Verzeichnissuche
    for ( int i = 0; i < targets.size(); ++i )
    {
      d.setPath ( targets.at ( i ) );
      if ( d.exists() )
      {
        foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
        {
          QString bn = info.completeBaseName();
          QRegExp pattern ( "^"+suffix+"\\-" );
          if ( bn.contains ( pattern ) )
          {
            list.append ( bn.replace ( pattern, "" ) );
          }
        }
      }
    }
    // Doppelte Einträge bei Symbolischen Verknüpfungen vermeiden!
    list.removeDuplicates();

    return list;
  }

  AVOptions::~AVOptions()
  {}

}  /* eof namespace QX11Grab */

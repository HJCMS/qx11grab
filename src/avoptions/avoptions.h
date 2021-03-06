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
**/

#ifndef QX11GRAB_AVOPTIONS_H
#define QX11GRAB_AVOPTIONS_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* FFmpeg */
extern "C"
{
#include <libavcodec/avcodec.h>
#ifdef QX11GRAB_OLD_AVCODEC_ID
# define AVCodecID CodecID
# define AV_CODEC_ID_NONE CODEC_ID_NONE
#endif
}

namespace QX11Grab
{
  /**
  * FFCodec
  * \ingroup Declarations
  */
  typedef struct Q_DECL_EXPORT
  {
    AVCodecID id;       /**< AV Codec ID from avcodec.h */
    QString name;       /**< Codec Name */
    QString fullname;   /**< Full Codec Description */
    QString info;       /**< Extra Codec Information for Tool Tips */
  } FFCodec;

  /**
  * FFOption
  * \ingroup Declarations
  */
  typedef struct Q_DECL_EXPORT
  {
    int id;           /**< Incremental Identifier for ComboBoxes */
    QString name;     /**< Parameter Name */
    QVariant value;   /**< Optional Default Value */
    QString help;     /**< Optional Helptext */
  } FFOption;

  /**
  * FFFormat
  * \ingroup Declarations
  */
  typedef struct Q_DECL_EXPORT
  {
    QString format; /**< codec name */
    QString description; /**< codec description */
    QString defaultExt; /** default extension */
    QVariant extensions; /**< all supported extensions */
  } FFFormat;

  /**
  * required for QStringList::join() and QString::split() in Table ItemViews
  * \ingroup Declarations
  */
  static const QString delimiter = QLatin1String ( "%" );

  /**
  * QX11Grab using -filter:v and \b not -vf for inserting filters!
  * \ingroup Declarations
  */
  static const QString avFilterPredicate = QLatin1String ( "-filter:v" );

  /**
  * QX11Grab using -filter:a and \b not -af for inserting filters!
  * \ingroup Declarations
  */
  static const QString acFilterPredicate = QLatin1String ( "-filter:a" );

  /**
  * base class for fetching FFmpeg Codecs/Formats and Options
  * \ingroup FFmpeg
  */
  class Q_DECL_EXPORT AVOptions : public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

    public:
      AVOptions ( QObject * parent = 0 );

      /** send a avcodec option request only VIDEO/AUDIO supported
      * \param _option  query option \b without leading hyphens
      * \param _codec   codec name
      */
      static const QList<FFOption> optionQuery ( const QByteArray &_option, const QByteArray &_codec );

      /** ffmpeg -sample_fmts */
      static const QList<FFOption> sampleFormats();

      /** ffmpeg -pix_fmts */
      static const QList<FFOption> pixelFormats();

      /** all supported AVMEDIA_TYPE_VIDEO encoder types */
      static const QList<FFCodec> videoCodecs();

      /** all supported AVMEDIA_TYPE_AUDIO encoder types */
      static const QList<FFCodec> audioCodecs();

      /** find valid Format for given codecID */
      static const QList<FFFormat> supportedFormats ( AVCodecID id );

      /** complete list of all ~/.ffmpeg/?.ffpreset */
      static const QStringList userPresets ( const QString &suffix );

      /** complete list of all /usr/share/ffmpeg/?.ffpreset */
      static const QStringList systemPresets ( const QString &suffix );

      virtual ~AVOptions();
  };

}  /* eof namespace QX11Grab */

Q_DECLARE_METATYPE ( QX11Grab::FFCodec )
Q_DECLARE_METATYPE ( QX11Grab::FFOption )
Q_DECLARE_METATYPE ( QX11Grab::FFFormat )

#endif

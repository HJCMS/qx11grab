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

#ifndef QX11OPTIONS_AVOPTIONS_H
#define QX11OPTIONS_AVOPTIONS_H

/* QtCore */
#include <QtCore/QGlobalStatic>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace QX11Options
{

  typedef struct Q_DECL_EXPORT
  {
    QString name;
    QString fullname;
  } FFCodec;

  typedef struct Q_DECL_EXPORT
  {
    int id;
    QString name;
    QVariant value;
  } FFOption;

  /**
  * base class for fetching FFmpeg Codecs and Options
  */
  class Q_DECL_EXPORT AVOptions : public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

    Q_SIGNALS:
      void optionInfo ( const QString &info );

    public:
      AVOptions ( QObject * parent = 0 );

      void getVideoCodecOption ( const QString &option, const QVariant &value = QVariant() );

      /** ffmpeg -aspect */
      static const QList<FFOption> aspect();

      /** ffmpeg -me_method */
      static const QList<FFOption> meMethod();

      /** ffmpeg -sample_fmts */
      static const QList<FFOption> sampleFormats();

      /** ffmpeg -pix_fmts */
      static const QList<FFOption> pixelFormats();

      /** all supported AVMEDIA_TYPE_VIDEO encoder types */
      static const QList<FFCodec> videoCodecs();

      /** all supported AVMEDIA_TYPE_AUDIO encoder types */
      static const QList<FFCodec> audioCodecs();

      virtual ~AVOptions();
  };

}  /* eof namespace QX11Options */

Q_DECLARE_METATYPE ( QX11Options::FFCodec )
Q_DECLARE_METATYPE ( QX11Options::FFOption )

#endif

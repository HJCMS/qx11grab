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
#include <QtCore/QVariant>

/* FFmpeg */
extern "C"
{
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

namespace QX11Options
{

  typedef struct Q_DECL_EXPORT
  {
    QString name;
    QString fullname;
  } FFCodec;

  /**
  * base class for fetching FFmpeg Codecs and Options
  */
  class Q_DECL_EXPORT AVOptions : public QObject
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

    Q_SIGNALS:
      void optionInfo ( const QString &info );

    public Q_SLOTS:
      void optionRequest ( const QString &option, const QVariant &value = QVariant() );

    public:
      AVOptions ( QObject * parent = 0 );

      /**
      * a list of all supported AVMEDIA_TYPE_VIDEO encoder types
      */
      static const QList<FFCodec> videoCodecs();

      /**
      * a list of all supported AVMEDIA_TYPE_AUDIO encoder types
      */
      static const QList<FFCodec> audioCodecs();

      virtual ~AVOptions();
  };

}  /* eof namespace QX11Options */

Q_DECLARE_METATYPE ( QX11Options::FFCodec )

#endif
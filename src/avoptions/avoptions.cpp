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
#include <libavutil/common.h>
}

namespace QX11Options
{

  AVOptions::AVOptions ( QObject * parent )
      : QObject ( parent )
  {
    setObjectName ( "AVOptions" );
  }

  void AVOptions::optionRequest ( const QString &option, const QVariant &value )
  {
    qDebug() << "TODO" << Q_FUNC_INFO << option << value;
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

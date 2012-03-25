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

#ifndef QX11GRAB_X11DEVICE_H
#define QX11GRAB_X11DEVICE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>

/* QX11Grab */
#include "avoptions.h"

/* FFmpeg */
extern "C"
{
#include <libavformat/avformat.h>
}

namespace QX11Grab
{
  /** x11 grabber device demuxer declaration
  AVInputFormat ff_x11_grab_device_demuxer =
  {
    .name           = "x11grab",
    .long_name      = NULL_IF_CONFIG_SMALL ( "X11grab" ),
    .priv_data_size = sizeof ( struct x11_grab ),
    .read_header    = x11grab_read_header,
    .read_packet    = x11grab_read_packet,
    .read_close     = x11grab_read_close,
    .flags          = AVFMT_NOFILE,
    .priv_class     = &x11_class,
  };
  */

  /**
  * X11 Device Demuxer
  */
  class Q_DECL_EXPORT X11Device : public QThread
  {
      Q_OBJECT
      Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
      Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

    private:
      /** Packet Bytes */
      unsigned char* m_data;
      AVCodecContext *libavcodec;

    protected:
      virtual void run();

    public:
      explicit X11Device ( QObject * parent = 0 );
      virtual ~X11Device();
  };

} /* eof namespace QX11Grab */

#endif

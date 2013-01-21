/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#ifndef WEBCAMDEVICEINFO_H
#define WEBCAMDEVICEINFO_H

/* QtCore */
#include <QtCore/QMetaType>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/Qt>

class Q_DECL_EXPORT WebCamDeviceInfo
{
  public:
    QString path;       /**< Absolute DevicePath */
    QString driver;     /**< Driver Name */
    QString card;       /**< Card Name */
    QString bus;        /**< SystemBus Name */
    QString pixfmt;     /**< Pixel Format */
    QSize   sourceSize; /**< Default Source Frame Size */
    QSize   outputSize; /**< Different Output Frame Size */
    WebCamDeviceInfo ()
        : path ( QString::null )
        , driver ( QString::null )
        , card ( QString::null )
        , bus ( QString::null )
        , pixfmt ( QString::null )
        , sourceSize ( QSize ( 160, 120 ) )
        , outputSize ( QSize ( 160, 120 ) )
    {};

    WebCamDeviceInfo ( const WebCamDeviceInfo &p )
        : path ( p.path )
        , driver ( p.driver )
        , card ( p.card )
        , bus ( p.bus )
        , pixfmt ( p.pixfmt )
        , sourceSize ( p.sourceSize )
        , outputSize ( p.outputSize )
    {};
};

Q_DECLARE_METATYPE ( WebCamDeviceInfo )

#endif

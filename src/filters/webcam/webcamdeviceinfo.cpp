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

#include "webcamdeviceinfo.h"

/* QtCore */
#include <QtCore/QDebug>

WebCamDeviceInfo::WebCamDeviceInfo ( const QString &devicePath,
                                     const QString &driverName,
                                     const QString &cardName,
                                     const QString &busName )
    : p_path ( devicePath )
    , p_driver ( driverName )
    , p_card ( cardName )
    , p_bus ( busName )
{}

const QString WebCamDeviceInfo::path()
{
  return p_path;
}

const QString WebCamDeviceInfo::driver()
{
  return p_driver;
}

const QString WebCamDeviceInfo::card()
{
  return p_card;
}

const QString WebCamDeviceInfo::bus()
{
  return p_bus;
}

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

#ifndef WEBCAMDEVICEINFO_H
#define WEBCAMDEVICEINFO_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QVariant>

/* QX11Grab */
#include "v4l2-api.h"

class WebCamDeviceInfo
{
  private:
    const QString p_path;
    const QString p_driver;
    const QString p_card;
    const QString p_bus;

  public:
    WebCamDeviceInfo ( const QString &devicePath,
                       const QString &driverName,
                       const QString &cardName,
                       const QString &busName
                     );

    const QString path();
    const QString driver();
    const QString card();
    const QString bus();
};

#endif

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

#include "webcamdevice.h"
#include "webcamdevicechooser.h"

/* libv4l2 */
#include <libv4l2.h>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QToolButton>
#include <QtGui/QGridLayout>

WebCamDevice::WebCamDevice ( QWidget * parent )
    : QWidget ( parent )
    , m_v4l2 ( new v4l2 )
{
  setObjectName ( QLatin1String ( "WebCamDevice" ) );

  int gridRow = 0;
  QGridLayout* layout = new QGridLayout ( this );
  layout->setObjectName ( QLatin1String ( "WebCamDevice/Layout" ) );

  m_deviceChooser = new WebCamDeviceChooser ( this );
  layout->addWidget ( m_deviceChooser, gridRow, 0, 1, 1, Qt::AlignLeft );

  setLayout ( layout );

  // Signals

}

void WebCamDevice::triggerDevices ( const QString &dev )
{
  Q_UNUSED ( dev ); // TODO Add Default Device
  m_deviceChooser->init();
}

WebCamDevice::~WebCamDevice()
{}

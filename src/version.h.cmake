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

#ifndef QX11GRAB_VERSION_H
#define QX11GRAB_VERSION_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QGlobalStatic>
#include <QtCore/QMap>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QX11Info>

/**
* @short Version
*/
#define QX11GRAB_VERSION "@QX11GRAB_VERSION@"

/**
* @short dbus Domain
*/
#define QX11GRAB_DBUS_DOMAIN_NAME "@QX11GRAB_DBUS_DOMAIN_NAME@"

/**
* @short recording logfile
*/
static inline const QString qx11grabLogfile()
{
  QString log;
  log.sprintf ( "qx11grab-%s_%s.log", getenv ( "USER" ), getenv ( "DISPLAY" ) );
  QDir dir ( QDir::tempPath() );
  return dir.filePath ( log );
}

/**
* @short Contact E-Mail
*/
static inline const char* aboutMail()
{
  QByteArray m ( "nospam" );
  m.append ( "@" );
  m.append ( "hjcms.de" );
  return m.constData();
}

#endif

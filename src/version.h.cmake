/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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
#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

/**
* @short Version
*/
#define QX11GRAB_VERSION "@QX11GRAB_VERSION@"

/**
* @short dbus Domain
*/
#define QX11GRAB_DBUS_DOMAIN_NAME "de.hjcms.qx11grab"

/**
* @short sound device is busy
*/
#define OSS_IN_USE "audio device %s is busy"

/**
* @short fallback function
* find Oxygen Theme Icon or receive it from qrc
*/
static inline const QIcon getThemeIcon ( const QString &icon )
{
  QMap<QString,QString> map;
  map["application-exit"] = "stop";
  map["audio-headset"] = "find";
  map["document-save"] = "save";
  map["edit-redo"] = "redo";
  map["folder-open"] = "folder";
  map["maximize"] = "maximize";
  map["media-record"] = "convert";
  map["media-playback-stop"] = "stop";
  map["minimize"] = "minimize";
  map["qx11grab"] = "qx11grab";
  map["view-refresh"] = "refresh";
  map["view-grid"] = "grid";
  map["window"] = "window";
  map["window-close"] = "stop";

  // qDebug() << key << map[key];
  QIcon fallbackIcon;
  QPixmap pixmap = QPixmap ( QString::fromUtf8 ( "://images/%1.png" ).arg ( map[icon] ) );
  fallbackIcon.addPixmap ( pixmap, QIcon::Normal, QIcon::Off );

  return QIcon::fromTheme ( icon, fallbackIcon );
}

#endif

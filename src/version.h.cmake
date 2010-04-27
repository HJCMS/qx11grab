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
* @short html Notice
*/
#define HTML_NOTICE "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n" \
"<html><head><meta name=\"qrichtext\" content=\"1\" />" \
"<style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head>" \
"<body style=\" font-family:'Sans Serif'; font-size:12pt; font-weight:400; font-style:normal;\">\n" \
"<p style=\" margin:0px; text-indent:0px;\"><span style=\" color:#ff0000;\">%1</span>: %2</p>" \
"</body></html>"

/**
* @short dbus Domain
*/
#define QX11GRAB_DBUS_DOMAIN_NAME "de.hjcms.qx11grab"

/**
* @short sound device is busy
*/
#define OSS_IN_USE "Audio Device %s already in use\nDisable Capture with -f oss Option"

/**
* @short fallback function
* find Theme Icon
*/
static inline const QIcon getThemeIcon ( const QString &key )
{
  QMap<QString,QString> map;
  // Oxygen
  map["application-exit"] = "stop";
  map["audio-headset"] = "find";
  map["document-save"] = "save";
  map["edit-redo"] = "redo";
  map["maximize"] = "maximize";
  map["media-record"] = "convert";
  map["media-playback-stop"] = "stop";
  map["minimize"] = "minimize";
  map["qx11grab"] = "qx11grab";
  map["view-refresh"] = "refresh";
  map["view-grid"] = "grid";
  map["window"] = "window";
  map["window-close"] = "stop";

  qDebug() << key << map[key];

  QIcon icon;
  QPixmap pixmap = QPixmap ( QString::fromUtf8 ( "://images/%1.png" ).arg ( map[key] ) );
  icon.addPixmap ( pixmap, QIcon::Normal, QIcon::Off );

  return QIcon::fromTheme ( key, icon );
}

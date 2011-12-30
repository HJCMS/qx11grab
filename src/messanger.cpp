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

#include "messanger.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QApplication>

/* QtDBus */
#include <QtDBus/QDBusReply>

Messanger::Messanger ( const QDBusConnection &connection, QObject * parent )
    : QDBusInterface ( QString ( "org.kde.knotify" ), QString ( "/Notify" ), "org.kde.KNotify", connection, parent )
{
  setObjectName ( QLatin1String ( "Messanger" ) );
}

void Messanger::sendMessage ( const QString &txt )
{
//   Body b;
//   b.event = QString ( "Info" );
//   b.fromApp = QString ( "qx11grab" );
//   b.contexts =  QVariantList();
//   b.title = QString ( "BusTest" );
//   b.text = txt;
//   b.pixmap =  QByteArray();
//   b.actions = QStringList();
//   b.timeout = 5000;
//   call ( "fromApp", QString ( "qx11grab" ) );
//   call ( "title", QString ( "BusTest" ) );
//   call ( "text", txt );
//   call ( "timeout", 5000 );
//   QDBusReply<QString> reply = asyncCall ( "event" );
//   qDebug() << reply.error();
  qDebug() << Q_FUNC_INFO << "TODO" << txt;
} 

Messanger::~Messanger()
{}

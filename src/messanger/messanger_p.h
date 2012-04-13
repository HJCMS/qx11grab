/*
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
*/

#ifndef MESSANGER_PRIVATE_H
#define MESSANGER_PRIVATE_H

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>
#include <QtGui/QImage>

/* QtDBus */
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMetaType>

/* DBus Interfaces */
#include "notificationsinterface.h"

/* QX11Grab */
#include "messanger.h"

/**
* qDBusRegisterMetaType Implementation
*/
QDBusArgument& operator<< ( QDBusArgument &arg, const QImage &image );
const QDBusArgument& operator>> ( const QDBusArgument &arg, QImage &image );

class MessangerPrivate
{
  private:
    Messanger* q_ptr;
    Q_DECLARE_PUBLIC ( Messanger )

  public:
    org::freedesktop::Notifications* iface;
    QVariantMap hints ( const QString &iconName = QString::fromUtf8 ( "qx11grab" ) ) const;

    explicit MessangerPrivate ( Messanger * p );
    ~MessangerPrivate() {};
};

#endif

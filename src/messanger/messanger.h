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

#ifndef MESSANGER_H
#define MESSANGER_H

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

class MessangerPrivate;

class Messanger : public QDBusInterface
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    Q_DECLARE_PRIVATE ( Messanger )
    Q_DISABLE_COPY ( Messanger )
    QDBusConnection p_dbus;
    QScopedPointer<MessangerPrivate> d_ptr;

  private Q_SLOTS:
    void notify ( const QString &type, const QString &title, const QString &body );

  protected:
    /**
    * Connect to Notification Daemon
    */
    bool createConnection();

  Q_SIGNALS:
    /**
    * this will emitted if no Notification Daemon reply errors
    */
    void replyMessage ( const QString &mess, int timeout = 5000 );

  public:
    explicit Messanger ( const QDBusConnection &connection, QObject * parent = 0 );
    /**
    * Send Info Message to Notification Daemon
    * @param title  Message Title
    * @param body Body Message
    */
    bool sendInfoMessage ( const QString &title, const QString &body );

    /**
    * Send Warning Message to Notification Daemon
    * @param title  Message Title
    * @param body Body Message
    */
    bool sendWarnMessage ( const QString &title, const QString &body );

    /**
    * Send Failure Message to Notification Daemon
    * @param title  Message Title
    * @param body Body Message
    */
    bool sendErrorMessage ( const QString &title, const QString &body );

    ~Messanger();
};

#endif

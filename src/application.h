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

#ifndef QX11GRAB_MAIN_H
#define QX11GRAB_MAIN_H

/* QX11Grab */
#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QX11Info>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

/* QtNetwork */
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

/* QX11Grab */
#include "settings.h"

class Application : public QApplication
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QLocalServer* m_server;
    QDBusConnection* m_dbus;
    Settings* m_settings;
    const QString localSocketName() const;
    const QString socketPath();
    bool createEnviroment();

  private Q_SLOTS:
    void newConnection();

  protected:
    bool startUniqueServer();

  Q_SIGNALS:
    void messageReceived ( QLocalSocket *socket );

  public:
    Application ( int &argc, char **argv );
    Settings* setting();
    QDBusConnection* bus();
    ~Application();
};

#endif

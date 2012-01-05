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

/* QX11Grab */
#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

#include "application.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>

Application::Application ( int &argc, char **argv )
    : QApplication ( argc, argv, true )
    , dbus ( 0 )
{
  Q_INIT_RESOURCE ( qx11grab );
  setApplicationName ( "qx11grab" );
  setApplicationVersion ( QX11GRAB_VERSION );
  setOrganizationDomain ( "hjcms.de" );
  setQuitOnLastWindowClosed ( false );
}

bool Application::start()
{
  QString reg ( QX11GRAB_DBUS_DOMAIN_NAME );
  dbus = new QDBusConnection ( QDBusConnection::sessionBus() );
  if ( ! dbus )
  {
    qFatal ( "can not connect to dbus" );
    return false;
  }
  return dbus->registerService ( reg );
}

Application::~Application()
{}

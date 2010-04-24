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

#include "qx11grabadaptor.h"

#include <QtCore/QDebug>
#include <QtCore/QGenericReturnArgument>

QX11GrabAdaptor::QX11GrabAdaptor ( QObject *parent )
    : QDBusAbstractAdaptor ( parent )
{
  rcDBus = new QDBusConnection ( QDBusConnection::sessionBus() );
  rcDBus->registerService ( "de.hjcms.qx11grab" );
  rcDBus->sessionBus().registerObject ( "/qx11grab", parent, QDBusConnection::ExportAdaptors );
  setAutoRelaySignals ( false );
}

void QX11GrabAdaptor::rubberband()
{
  QMetaObject::invokeMethod ( parent(), "swapRubberBand" );
}

void QX11GrabAdaptor::stop()
{
  QMetaObject::invokeMethod ( parent(), "stop" );
}

void QX11GrabAdaptor::start()
{
  QMetaObject::invokeMethod ( parent(), "record" );
}

void QX11GrabAdaptor::show ()
{
  QMetaObject::invokeMethod ( parent(), "showNormal" );
}

void QX11GrabAdaptor::hide ()
{
  QMetaObject::invokeMethod ( parent(), "hide" );
}

const QString QX11GrabAdaptor::command()
{
  QString cmd;
  QMetaObject::invokeMethod ( parent(), "currentCommandLine", Q_RETURN_ARG ( QString, cmd ) );
  return cmd;
}
/*
const QString QX11GrabAdaptor::Option ( const QString &key )
{
  QString val;
  QMetaObject::invokeMethod ( parent(), "getSettingsValue",
                              Q_RETURN_ARG ( QString, val ),
                              Q_ARG ( QString, key ) );
  return val;
}
*/

QX11GrabAdaptor::~QX11GrabAdaptor()
{
  rcDBus->unregisterObject ( "/qx11grab", QDBusConnection::UnregisterNode );
}

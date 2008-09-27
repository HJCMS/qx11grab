/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "qx11grabadaptor.h"

#include <QtCore/QString>
#include <QtCore/QDebug>

QX11GrabAdaptor::QX11GrabAdaptor ( QObject *parent )
    : QDBusAbstractAdaptor ( parent )
{
  rcDBus = new QDBusConnection ( QDBusConnection::sessionBus() );
  rcDBus->registerService ( "de.hjcms" );
  rcDBus->sessionBus().registerObject ( "/qx11grab", parent, QDBusConnection::ExportAdaptors );
  setAutoRelaySignals ( false );
}

void QX11GrabAdaptor::stop()
{
    QMetaObject::invokeMethod(parent(), "stop_record");
}

void QX11GrabAdaptor::start()
{
    QMetaObject::invokeMethod(parent(), "start_record");
}

QX11GrabAdaptor::~QX11GrabAdaptor()
{
  rcDBus->unregisterObject ( "/qx11grab", QDBusConnection::UnregisterNode );
}

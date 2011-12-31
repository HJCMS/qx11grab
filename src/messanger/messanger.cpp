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
#include "messanger_p.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

/* QtDBus */
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusReply>

/**
* qDBusRegisterMetaType Implementation
* @{
*/
QDBusArgument& operator<< ( QDBusArgument &arg, const QImage &image )
{
  if ( image.isNull() )
  {
    // Sometimes this gets called with a null QImage for no obvious reason.
    arg.beginStructure();
    arg << 0 << 0 << 0 << false << 0 << 0 << QByteArray();
    arg.endStructure();
    return arg;
  }
  QImage scaled = image.scaledToHeight ( 100, Qt::SmoothTransformation );
  QImage i = scaled.convertToFormat ( QImage::Format_ARGB32 ).rgbSwapped();
  arg.beginStructure();
  arg << i.width();
  arg << i.height();
  arg << i.bytesPerLine();
  arg << i.hasAlphaChannel();
  int channels = i.isGrayscale() ? 1 : ( i.hasAlphaChannel() ? 4 : 3 );
  arg << i.depth() / channels;
  arg << channels;
  arg << QByteArray ( reinterpret_cast<const char*> ( i.bits() ), i.numBytes() );
  arg.endStructure();
  return arg;
}

const QDBusArgument& operator>> ( const QDBusArgument &arg, QImage &image )
{
  Q_UNUSED ( image );
  Q_ASSERT ( 0 ); // This is needed to link but shouldn't be called.
  return arg;
}

/**
* @}
*/

/** \class MessangerPrivate */
MessangerPrivate::MessangerPrivate ( Messanger * p )
    : q_ptr ( p )
    , iface ( 0 )
    , notifyID ( 0 )
{}

QVariantMap MessangerPrivate::hints ( const QString &iconName ) const
{
  QVariantMap hints;
  QIcon icon = QIcon::fromTheme ( qApp->applicationName() );
  if ( icon.isNull() )
    icon = QIcon::fromTheme ( iconName );

  QPixmap pixmap = icon.pixmap ( 32, QIcon::Normal, QIcon::On );
  QImage image = pixmap.toImage();
  if ( ! image.isNull() )
  {
    QVariant imageData;
    imageData.setValue ( image );
    qDBusRegisterMetaType<QImage>();
    hints["image_data"] = imageData;
  }
  return hints;
}

/** \class Messanger */
Messanger::Messanger ( const QDBusConnection &connection, QObject * parent )
    : QDBusInterface ( QString ( "de.hjcms.qx11grab" ),
                       QString ( "/" ), "de.hjcms.qx11grab",
                       connection, parent )
    , p_dbus ( connection )
    , d_ptr ( new MessangerPrivate ( this ) )
{
  setObjectName ( QLatin1String ( "Messanger" ) );
}

void Messanger::finished ( QDBusPendingCallWatcher * watcher )
{
  Q_D ( Messanger );
  QDBusPendingReply<uint> reply = *watcher;
  if ( reply.isError() )
  {
    qWarning() << "Error sending notification" << reply.error().name();
    return;
  }

  quint32 id = reply.value();
  if ( id != 0 )
  {
    d->notifyID = id;
    d->lastNotifyTime = QDateTime::currentDateTime();
  }
}

void Messanger::notify ( const QString &type, const QString &title, const QString &body )
{
  Q_D ( Messanger );

  qint32 id = 0;
  qint32 timeout = 5000; // 5 Sekunden
  if ( ( d->lastNotifyTime.secsTo ( QDateTime::currentDateTime() ) * 1000 ) < timeout )
  {
    /* Wenn in KDE4 ein Nachricht bereits gesendet wurde.
    *  Müssen wir erst die ID zerstören damit ältere
    *  Nachrichten nicht noch einmal anzeigt! */
    id = d->notifyID;
  }

  QString appName = qApp->applicationName();
  QDBusPendingReply<uint> reply = d->iface->Notify (
                                      appName,
                                      id,
                                      appName,
                                      title,
                                      body,
                                      QStringList (),
                                      d->hints ( type ),
                                      timeout
                                  );

  QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher ( reply, this );
  connect ( watcher, SIGNAL ( finished ( QDBusPendingCallWatcher* ) ),
            this, SLOT ( finished ( QDBusPendingCallWatcher* ) ) );

  if ( reply.isError() )
  {
    qWarning ( "Notification Failure: %s\n%s\n",
               qPrintable ( reply.error().name() ),
               qPrintable ( reply.error().message() ) );

    emit errors ( trUtf8 ( "Notification not send!" ) );
  }
}

bool Messanger::createConnection()
{
  Q_D ( Messanger );
  d->iface = new org::freedesktop::Notifications (
      QString ( "org.freedesktop.Notifications" ),
      QString ( "/org/freedesktop/Notifications" ), p_dbus, this );

  // qDebug() << Q_FUNC_INFO << d->iface->lastError().name();

  return d->iface->isValid();
}

void Messanger::sendInfoMessage ( const QString &title, const QString &body )
{
  if ( ! createConnection() )
  {
    qWarning() << "Notification Daemon not available!";
    return;
  }
  notify ( "dialog-information", title, body );
}

void Messanger::sendWarnMessage ( const QString &title, const QString &body )
{
  if ( ! createConnection() )
  {
    qWarning() << "Notification Daemon not available!";
    return;
  }
  notify ( "dialog-warning", title, body );
}

void Messanger::sendErrorMessage ( const QString &title, const QString &body )
{
  if ( ! createConnection() )
  {
    qWarning() << "Notification Daemon not available!";
    return;
  }
  notify ( "dialog-error", title, body );
}

Messanger::~Messanger()
{}
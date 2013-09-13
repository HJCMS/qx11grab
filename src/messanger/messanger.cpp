/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "messanger.h"
#include "icon_p.h"
#include "messanger_p.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QProcess>

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
{}

QVariantMap MessangerPrivate::hints ( const QString &iconName ) const
{
  Q_UNUSED ( iconName );
  QVariantMap hints;
  QImage image ( icon_size192, 192, 192, 24, QImage::Format_ARGB8565_Premultiplied );
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

bool Messanger::notify_send ( const QString &type, const QString &title, const QString &body )
{
#ifdef NOTIFYSEND_BINARY
  QStringList args;
  args << "-a" << qApp->applicationName();
  args << "-t" << "1500"; // 2,5 Sekunden
  args << "-i" << qApp->applicationName();
  args << "-c" << type;
  args << QString::fromUtf8 ( "\"%1\"" ).arg ( title );
  args << QString::fromUtf8 ( "\"%1\"" ).arg ( body );
  QProcess process ( this );
  return process.startDetached ( "notify-send", args );
#endif

  return false;
}

void Messanger::notify ( const QString &type, const QString &title, const QString &body )
{
  Q_D ( Messanger );

  qint32 id = 0;
  qint32 timeout = 1500; // 2,5 Sekunden

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

  if ( reply.isError() )
  {
    qWarning ( "Notification Failure: %s\n%s\n",
               qPrintable ( reply.error().name() ),
               qPrintable ( reply.error().message() ) );

    emit replyMessage ( trUtf8 ( "Notification not send!" ) );
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

bool Messanger::sendInfoMessage ( const QString &title, const QString &body )
{
  if ( ! createConnection() )
    return notify_send ( "dialog-information", title, body );

  notify ( "dialog-information", title, body );
  return true;
}

bool Messanger::sendWarnMessage ( const QString &title, const QString &body )
{
  if ( ! createConnection() )
    return notify_send ( "dialog-warning", title, body );

  notify ( "dialog-warning", title, body );
  return true;
}

bool Messanger::sendErrorMessage ( const QString &title, const QString &body )
{
  if ( ! createConnection() )
    return notify_send ( "dialog-error", title, body );

  notify ( "dialog-error", title, body );
  return true;
}

Messanger::~Messanger()
{}

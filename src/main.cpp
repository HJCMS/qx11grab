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

#include <cstdlib>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QTextCodec>
#include <QtCore/QStringList>
#include <QtCore/QLibraryInfo>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>

#include "version.h"
#include "settings.h"
#include "qx11grab.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

#ifdef Q_OS_UNIX
#include <QX11Info>
#endif

bool isAlreadyRunning()
{
  QString reg ( "de.hjcms.qx11grab" );
  QDBusConnection* bus = new QDBusConnection ( QDBusConnection::sessionBus() );
  if ( ! bus->registerService ( reg ) )
  {
    qWarning ( "QX11Grab Already Running" );
    QDBusConnection dbus = bus->connectToBus ( QDBusConnection::SessionBus, reg );
    QDBusMessage meth = QDBusMessage::createMethodCall ( reg, "/qx11grab", reg, "show" );
    if ( dbus.send ( meth ) )
      dbus.disconnectFromBus ( reg );

    return true;
  }
  return false;
}

int main ( int argc, char *argv[] )
{
  if ( isAlreadyRunning() )
    return EXIT_SUCCESS;

  Q_INIT_RESOURCE ( qx11grab );

  QT_REQUIRE_VERSION ( argc, argv, "4.6.0" )
  QApplication app ( argc, argv, true );
  app.setApplicationName ( "qx11grab" );
  app.setApplicationVersion ( QX11GRAB_VERSION );
  app.setOrganizationDomain ( "hjcms.de" );

  if ( ! QSystemTrayIcon::isSystemTrayAvailable() )
  {
    QMessageBox::critical ( 0, "Systray", "I couldn't detect any system tray." );
    return EXIT_FAILURE;
  }
  QApplication::setQuitOnLastWindowClosed ( false );
  QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

  QStringList transpaths ( QCoreApplication::applicationDirPath () );
  transpaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator;
  foreach ( QString d, transpaths )
  {
    if ( translator.load ( QString ( "%1/qx11grab_%2" ).arg ( d, QLocale().name() ) ) )
      break;
  }
  app.installTranslator ( &translator );

  Settings *m_Settings = new Settings ( &app );

  QX11Grab grab ( m_Settings );

  if ( m_Settings->getBool ( "startMinimized" ) )
    grab.hide();
  else
    grab.show();

  return app.exec();
}

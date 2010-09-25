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

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

#include <cstdlib>

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QTranslator>
#include <QtCore/QTextCodec>

/* QtGui */
#include <QtGui/QMessageBox>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QX11Info>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

#include "settings.h"
#include "qx11grab.h"

/* KDE */
#ifdef HAVE_KDE4_SUPPORT
# include <KDE/KAboutData>
# include <KDE/KUniqueApplication>
# include <KDE/KCmdLineArgs>
#else
# include <QtGui/QApplication>
#endif

int main ( int argc, char* argv[] )
{
#ifdef HAVE_KDE4_SUPPORT

  KAboutData about ( "qx11grab", "qx11grab", ki18n ( "qx11grab" ), QX11GRAB_VERSION,
                     ki18n ( "A Desktop Recording Tool" ),
                     KAboutData::License_LGPL,
                     ki18n ( "Copyright (C) 2010 Developer" ) );

  KCmdLineArgs::init ( argc, argv, &about );
  KUniqueApplication app ( true );

#else

  QString reg ( QX11GRAB_DBUS_DOMAIN_NAME );
  QDBusConnection* bus = new QDBusConnection ( QDBusConnection::sessionBus() );
  if ( ! bus->registerService ( reg ) )
  {
    qWarning ( "QX11Grab Already Running" );
    QDBusConnection dbus = bus->connectToBus ( QDBusConnection::SessionBus, reg );
    QDBusMessage meth = QDBusMessage::createMethodCall ( reg, "/qx11grab", reg, "show" );
    if ( dbus.send ( meth ) )
      dbus.disconnectFromBus ( reg );

    return EXIT_SUCCESS;
  }
  QApplication app ( argc, argv, true );

#endif

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

  Settings* m_Settings = new Settings ( &app );

  QX11Grab grab ( m_Settings );

  if ( m_Settings->value ( "startMinimized", false ).toBool() )
    grab.hide();
  else
    grab.show();

  return app.exec();
}

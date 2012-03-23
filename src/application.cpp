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
#include "adaptor.h"
#include "settings.h"
#include "mainwindow.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QGlobalStatic>

/* QtGui */
#include <QtGui/QIcon>

/* KDE4 */
#ifdef HAVE_KDE4_SUPPORT
# include <KDE/KAboutData>
# include <KDE/KLocale>
# include <KDE/KLocalizedString>
#endif

Application::Application ( int &argc, char **argv )
    : QApplication ( argc, argv, QApplication::GuiClient )
    , connected ( false )
    , m_settings ( 0 )
    , m_dbus ( 0 )
    , m_window ( 0 )
{
  Q_INIT_RESOURCE ( qx11grab );
  setApplicationName ( "qx11grab" );
  setApplicationVersion ( QX11GRAB_VERSION );
  setOrganizationDomain ( "hjcms.de" );
  setQuitOnLastWindowClosed ( false );

  m_settings = new Settings ( this );

  // BUG Qt >= 4.8 and IconTheme Paths
  QIcon iconTheme;
  QDir dir ( QString::fromUtf8 ( "%1/../share/icons" ).arg ( qApp->applicationDirPath() ) );
  if ( dir.exists() )
  {
    QStringList thlist ( dir.absolutePath() );
    iconTheme.setThemeSearchPaths ( thlist );
  }
  QString userIconTheme = m_settings->value ( "IconTheme", "oxygen" ).toString();
  if ( ! iconTheme.hasThemeIcon ( userIconTheme ) )
    iconTheme.setThemeName ( userIconTheme );

  QString engine = m_settings->value ( "GraphicsSystem", "native" ).toString();
  setGraphicsSystem ( engine );

#ifdef HAVE_OPENGL

  if ( engine.contains ( "opengl", Qt::CaseInsensitive ) )
  {
    if ( ! isEffectEnabled ( Qt::UI_AnimateToolBox ) )
      setEffectEnabled ( Qt::UI_AnimateToolBox, true );
  }

#endif

#ifdef HAVE_KDE4_SUPPORT
  QByteArray appsName = applicationName().toAscii();
  KAboutData kdata ( appsName, appsName,
                     ki18n ( "QX11Grab" ),
                     QX11GRAB_VERSION,
                     ki18n ( "X11 Desktop Recording" ),
                     KAboutData::License_LGPL_V3,
                     ki18n ( "Juergen Heinemann (Undefined)" ),
                     ki18n ( "Besides QX11Grab to tap Window Dimensions from X11 Environment and  generate Command-line options for FFmpeg." ),
                     QByteArray ( "http://qx11grab.hjcms.de" ),
                     m_settings->aboutMail()
                   );
  KLocale::setMainCatalog ( appsName );
#endif
}

/**
* Sicher gehen das laufenden Aufnahmen mit der Sitzung beendet werden!
* Danach von der DBus Session abmelden!
* @note Damit die Desktopumgebung keine Sperre erhält \b muss Unbedingt
* ein QSessionManager::release() gesetzt werden!
*/
void Application::commitData ( QSessionManager &manager )
{
  if ( m_window )
  {
    m_window->stop();
    m_dbus->unregisterObject ( QString ( "/" ), QDBusConnection::UnregisterTree );
  }
  manager.release();
}

/**
* NOTE DBus kann erst nach QApplication Initialisiert werden!
* Wenn die DBus Schnittstelle noch nicht gesetzt ist wird sie
* Registriert. Bei einem Fehlschlag wird false zurück geben.
* Die Methode setzt zusätzlich die Variable \ref connected
* die unter anderem auch von \ref createWindow verwendet wird.
*/
bool Application::start()
{
  QString reg ( QX11GRAB_DBUS_DOMAIN_NAME );
  m_dbus = new QDBusConnection ( QDBusConnection::sessionBus() );
  if ( ! m_dbus )
  {
    qFatal ( "can not connect to dbus" );
    connected = false;
  }
  connected = m_dbus->registerService ( reg );
  return connected;
}

/**
* Wenn eine DBus Verbindung besteht kann der Adapter an die
* Schnittstelle gesendet werden und das Fenster wird gestartet.
*/
void Application::createWindow()
{
  if ( ! connected )
    return;

  m_window = new  MainWindow ( m_settings );
  new Adaptor ( m_window );
  m_dbus->registerObject ( QString ( "/" ), m_window, ( QDBusConnection::ExportAdaptors ) );
  m_window->registerMessanger ( m_dbus );
}

Application::~Application()
{}

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
#include <QtCore/QGlobalStatic>

/* QtGui */
#include <QtGui/QIcon>

Application::Application ( int &argc, char **argv )
    : QApplication ( argc, argv, QApplication::GuiClient )
    , connected ( false )
    , m_window ( 0 )
    , m_settings ( 0 )
    , m_dbus ( 0 )
{
  Q_INIT_RESOURCE ( qx11grab );
  setApplicationName ( "qx11grab" );
  setApplicationVersion ( QX11GRAB_VERSION );
  setOrganizationDomain ( "hjcms.de" );
  setQuitOnLastWindowClosed ( false );

  m_settings = new Settings ( this );

  QIcon iconTheme; // BUG Qt >= 4.8
  QString userIconTheme = m_settings->value ( "IconTheme", "oxygen" ).toString();
  if ( ! iconTheme.hasThemeIcon ( userIconTheme ) )
    iconTheme.setThemeName ( userIconTheme );

  QString engine = m_settings->value ( "GraphicsSystem", "native" ).toString();
  setGraphicsSystem ( engine );

#ifdef HAVE_OPENGL

  if ( engine.compare ( "opengl" ) )
  {
    if ( ! isEffectEnabled ( Qt::UI_AnimateToolBox ) )
      setEffectEnabled ( Qt::UI_AnimateToolBox, true );
  }

#endif

  connect ( this, SIGNAL ( saveStateRequest ( QSessionManager & ) ),
            this, SLOT ( configureSession ( QSessionManager & ) ),
            Qt::DirectConnection );
}

void Application::configureSession ( QSessionManager &manager )
{
  QStringList cmd ( applicationName() );
  cmd << "-session" << manager.sessionId();
  m_settings->setValue ( "SessionManager", cmd );
  manager.setRestartCommand ( cmd );
  if ( m_dbus )
  {
    // qDebug() << Q_FUNC_INFO << m_dbus->baseService() << m_dbus->name();
    manager.setManagerProperty ( "baseService", m_dbus->baseService() );
  }
}

/**
* Wenn eine DBus Verbindung besteht kann der Adapter an die
* Schnittstelle gesendet werden und das Fenster wird gestartet.
*/
void Application::commitData ( QSessionManager &manager )
{
  if ( manager.allowsInteraction() )
  {
    if ( m_settings->contains ( "SessionManager" ) )
      manager.setDiscardCommand ( m_settings->value ( "SessionManager" ).toStringList() );


    manager.cancel();
  }
  else
  {
    QStringList cmd ( applicationName() );
    cmd << "-session" << manager.sessionId();
    manager.setRestartCommand ( cmd );
    manager.release();
  }
  m_settings->remove ( "SessionManager" );
}


bool Application::event ( QEvent * e )
{
  if ( e->type() == QEvent::Close )
    qDebug() << Q_FUNC_INFO << e->type();

  return true;
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

  if ( m_settings->value ( "startMinimized", false ).toBool() )
    m_window->hide();
  else
    m_window->show();
}

Application::~Application()
{
  if ( m_window )
  {
    m_dbus->unregisterObject ( QString ( "/" ), QDBusConnection::UnregisterTree );
    delete m_window;
  }
}

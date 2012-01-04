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

#include "application.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QGlobalStatic>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QtCore/QTextCodec>

Application::Application ( int &argc, char **argv )
    : QApplication ( argc, argv, true )
    , m_server ( 0 )
    , m_dbus ( 0 )
    , m_settings ( 0 )
{
  Q_INIT_RESOURCE ( qx11grab );
  setObjectName ( "qx11grab" );
  setApplicationName ( "qx11grab" );
  setApplicationVersion ( QX11GRAB_VERSION );
  setOrganizationDomain ( "hjcms.de" );
  setQuitOnLastWindowClosed ( false );

  m_settings = new Settings ( this );

  QIcon iconTheme; // BUG Qt >= 4.8
  QString userIconTheme = m_settings->value ( "IconTheme", "oxygen" ).toString();
  if ( ! iconTheme.hasThemeIcon ( userIconTheme ) )
    iconTheme.setThemeName ( userIconTheme );

  if ( startUniqueServer() )
    createEnviroment();
}

/**
* Setzt den lokalen Socket Namen für den Descriptor
*/
const QString Application::localSocketName() const
{
  QString name = applicationName();
  name.append ( "-" );
  name.append ( QString::number ( getuid() ) );
  name.append ( ".pid" );
  return name;
}

const QString Application::socketPath()
{
  QDir d ( QDir::tempPath() );
  return d.absoluteFilePath ( localSocketName() );
}

bool Application::createEnviroment()
{
  QString reg ( QX11GRAB_DBUS_DOMAIN_NAME );
  m_dbus = new QDBusConnection ( QDBusConnection::sessionBus() );
  if ( ! m_dbus->registerService ( reg ) )
  {
    qWarning ( "can not register dbus session" );
    return false;
  }

  if ( ! QSystemTrayIcon::isSystemTrayAvailable() )
  {
    QMessageBox::critical ( 0, "Systray", "I couldn't detect any system tray." );
    return false;
  }
  QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

  QStringList transpaths ( QCoreApplication::applicationDirPath () );
  transpaths << QLibraryInfo::location ( QLibraryInfo::TranslationsPath );

  QTranslator translator;
  foreach ( QString d, transpaths )
  {
    if ( translator.load ( QString ( "%1/qx11grab_%2" ).arg ( d, QLocale().name() ) ) )
      break;
  }
  installTranslator ( &translator );

  return true;
}

void Application::newConnection()
{
  QLocalSocket* soc = m_server->nextPendingConnection();
  if ( ! soc )
    return;

  soc->waitForReadyRead();
  soc->setSocketDescriptor ( getuid(), QLocalSocket::ConnectingState, QIODevice::ReadOnly);
  delete soc;
}

bool Application::startUniqueServer()
{
  if ( m_server )
    return false;

  bool isUp = false;
  m_server = new QLocalServer ( this );
  m_server->setMaxPendingConnections ( 1 );
  connect ( m_server, SIGNAL ( newConnection() ),
            this, SLOT ( newConnection() ) );

  if ( ! m_server->listen ( localSocketName() ) )
  {
    if ( QAbstractSocket::AddressInUseError == m_server->serverError() )
    {
      if ( QFile::exists ( socketPath() ) )
        QFile::remove ( socketPath() );

      if ( m_server->listen ( localSocketName() ) )
        isUp = true;
    }
  }
  else
    isUp = true;

  if ( isUp )
  {
    QFile f ( m_server->fullServerName() );
    if ( ! f.setPermissions ( ( QFile::ReadUser | QFile::WriteUser ) ) )
      qWarning() << "(qx11grab) Permissions Denied:" << f.fileName() << f.errorString();
  }
  else
  {
    qWarning() << "(qx11grab) Unable to listen:" << m_server->errorString();
    delete m_server;
    m_server = 0;
  }

  return isUp;
}

Settings* Application::setting()
{
  return m_settings;
}

QDBusConnection* Application::bus()
{
  return m_dbus;
}

Application::~Application()
{}

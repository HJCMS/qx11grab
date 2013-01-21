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
**/

/* QX11Grab */
#include "version.h"

#include "systemtray.h"
#include "settings.h"
#include "menubar.h"
#include "messanger.h"
#include "navigator.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMenu>

SystemTray::SystemTray ( MainWindow * parent )
    : QSystemTrayIcon ( parent )
    , m_mainWindow ( parent )
    , m_messanger ( 0 )
    , m_navigator ( new Navigator ( qApp->desktop() ) )
{
  setObjectName ( QLatin1String ( "SystemTray" ) );
  setIcon ( Settings::themeIcon ( "qx11grab" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "qx11grab: recording X11 Windows with ffmpeg" ) );

  QMenu* m_menu = new QMenu ( m_mainWindow );
  QAction* grabActionFromWindow = MenuBar::grabbingAction ( this );
  m_menu->addAction ( grabActionFromWindow );

  QAction* showRubberbandWindow = MenuBar::rubberbandAction ( this );
  m_menu->addAction ( showRubberbandWindow );

  QAction* m_actionNavi = m_menu->addAction ( trUtf8 ( "Navigation" ) );
  m_actionNavi->setObjectName ( QLatin1String ( "navigationAction" ) );
  /*: ToolTip */
  m_actionNavi->setStatusTip ( trUtf8 ( "to navigate" ) );
  m_actionNavi->setIcon ( Settings::themeIcon ( "window-new" ) );
  m_menu->addAction ( m_actionNavi );

  m_menu->addSeparator();

  m_actionStartRecord = MenuBar::startRecordAction ( this );
  m_menu->addAction ( m_actionStartRecord );

  m_actionStopRecord = MenuBar::stopRecordAction ( this );
  m_menu->addAction ( m_actionStopRecord );
  m_menu->addSeparator();

  QAction* minimizeWindowAction = MenuBar::hideWindowAction ( this );
  m_menu->addAction ( minimizeWindowAction );

  QAction* displayWindowAction = MenuBar::showWindowAction ( this );
  m_menu->addAction ( displayWindowAction );
  m_menu->addSeparator();

  QAction* quitWindowAction = MenuBar::quitAction ( this );
  m_menu->addAction ( quitWindowAction );

  setContextMenu ( m_menu );

  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( "/SystemTray", this, ( QDBusConnection::ExportScriptableSlots ) );

  connect ( grabActionFromWindow, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( grabFromWindow() ) );

  connect ( showRubberbandWindow, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( swapRubberBand() ) );

  connect ( m_actionStartRecord, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( startRecord() ) );

  connect ( m_actionStopRecord, SIGNAL ( triggered() ),
            m_mainWindow, SIGNAL ( stopRecording () ) );

  connect ( minimizeWindowAction, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( hide() ) );

  connect ( displayWindowAction, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( showNormal() ) );

  connect ( quitWindowAction, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( shutdown() ) );

  // Navigation
  connect ( m_actionNavi, SIGNAL ( triggered() ),
            this, SLOT ( showNavigator() ) );

  connect ( m_navigator, SIGNAL ( startRecord() ),
            m_mainWindow, SLOT ( startRecord() ) );

  connect ( m_navigator, SIGNAL ( stopRecord() ),
            m_mainWindow, SIGNAL ( stopRecording () ) );

  connect ( m_navigator, SIGNAL ( rubberBand() ),
            m_mainWindow, SLOT ( swapRubberBand() ) );

  connect ( m_navigator, SIGNAL ( hideMainWindow() ),
            m_mainWindow, SLOT ( hide() ) );

  connect ( m_navigator, SIGNAL ( showMainWindow() ),
            m_mainWindow, SLOT ( showNormal() ) );

}

/*
* Öffnen das Navigations Fenster
*/
void SystemTray::showNavigator()
{
  if ( m_navigator->isVisible() )
    m_navigator->hide();
  else
    m_navigator->show();
}

/*
* Aktiviert/Deaktiviert die Menüeinträge für die Aufnahme
*/
void SystemTray::setActionsEnabled ( bool b )
{
  m_actionStopRecord->setEnabled ( b );
  m_actionStartRecord->setEnabled ( ( ( b ) ? false : true ) );
  m_navigator->setActivity ( b );
}

/*
* Methode zum Ändern des Tooltips!
*/
void SystemTray::setCustomToolTip ( const QString &txt )
{
  setToolTip ( txt );
  m_navigator->setInfo ( txt );
}

/*
* Fehlermeldung weiter leiten
*/
void SystemTray::applicationMessage ( const QString &title, const QString &txt )
{
  sendMessage ( title, txt, QSystemTrayIcon::Warning );
}

/*
* Aktiviert den Messanger
*/
void SystemTray::setMessanger ( QDBusConnection* bus )
{
  m_messanger = new Messanger ( *bus, this );
  if ( ! m_messanger->isValid() )
  {
    qWarning() << "Messanger not available!";
    return;
  }
  connect ( m_messanger, SIGNAL ( replyMessage ( const QString &, int ) ),
            m_mainWindow, SLOT ( statusBarMessage ( const QString &, int ) ) );

#ifdef MAINTAINER_REPOSITORY
  m_messanger->sendInfoMessage ( "Information", "QX11Grab connected to DBus Notifications." );
#endif
}

/*
* Wenn eine Verbindung zum Messanger besteht diesen verwenden.
* Wenn nicht verwende \ref QSystemTrayIcon::showMessage
*/
void SystemTray::sendMessage ( const QString &title, const QString &message,
                               QSystemTrayIcon::MessageIcon icon )
{
  int timeout = 5000;
  if ( m_messanger )
  {
    switch ( icon )
    {
      case QSystemTrayIcon::Information:
      {
        if ( ! m_messanger->sendInfoMessage ( title, message ) )
          showMessage ( title, message, QSystemTrayIcon::Information, timeout );
      }
      return;

      case QSystemTrayIcon::Warning:
      {
        if ( ! m_messanger->sendWarnMessage ( title, message ) )
          showMessage ( title, message, QSystemTrayIcon::Warning, timeout );
      }
      return;

      case QSystemTrayIcon::Critical:
      {
        if ( ! m_messanger->sendErrorMessage ( title, message ) )
          showMessage ( title, message, QSystemTrayIcon::Critical, timeout );
      }
      return;

      default:
        showMessage ( title, message, QSystemTrayIcon::Information, timeout );
        return;
    }
  }
}

SystemTray::~SystemTray()
{}

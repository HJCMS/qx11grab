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

#include "systemtray.h"
#include "menubar.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QMenu>

SystemTray::SystemTray ( QX11Grab * parent )
    : QSystemTrayIcon ( parent )
    , mainWindow ( parent )
{
  setObjectName ( QLatin1String ( "SystemTray" ) );
  setIcon ( getThemeIcon ( "qx11grab" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "qx11grab: recording X11 Windows with ffmpeg" ) );

  QMenu* m_menu = new QMenu ( parent );
  QAction* grabActionFromWindow = MenuBar::grabbingAction ( this );
  m_menu->addAction ( grabActionFromWindow );

  QAction* showRubberbandWindow = MenuBar::rubberbandAction ( this );
  m_menu->addAction ( showRubberbandWindow );
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

  connect ( grabActionFromWindow, SIGNAL ( triggered() ),
            mainWindow, SLOT ( grabFromWindow() ) );

  connect ( showRubberbandWindow, SIGNAL ( triggered() ),
            mainWindow, SLOT ( swapRubberBand() ) );

  connect ( m_actionStartRecord, SIGNAL ( triggered() ),
            mainWindow, SLOT ( startRecord() ) );

  connect ( m_actionStopRecord, SIGNAL ( triggered() ),
            mainWindow, SIGNAL ( stopRecording () ) );

  connect ( minimizeWindowAction, SIGNAL ( triggered() ),
            mainWindow, SLOT ( hide() ) );

  connect ( displayWindowAction, SIGNAL ( triggered() ),
            mainWindow, SLOT ( showNormal() ) );

  connect ( quitWindowAction, SIGNAL ( triggered() ),
            qApp, SLOT ( quit() ) );
}

void SystemTray::setActionsEnabled ( bool b )
{
  m_actionStopRecord->setEnabled ( b );
  m_actionStartRecord->setEnabled ( ( ( b ) ? false : true ) );
}

SystemTray::~SystemTray()
{}

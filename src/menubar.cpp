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

#include "menubar.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QUrl>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

MenuBar::MenuBar ( QX11Grab * parent )
    : QMenuBar ( parent )
    , mainWindow ( parent )
{
  setObjectName ( QLatin1String ( "MenuBar" ) );
  m_menuFile = addMenu ( trUtf8 ( "File" ) );

  m_actionGrabbing = grabbingAction ( this, true );
  m_menuFile->addAction ( m_actionGrabbing );

  m_actionStartRecord = startRecordAction ( this, true );
  m_menuFile->addAction ( m_actionStartRecord );

  m_actionStopRecord = stopRecordAction ( this, true );
  m_menuFile->addAction ( m_actionStopRecord );

  m_actionKillRecord = killRecordAction ( this, true );
  m_menuFile->addAction ( m_actionKillRecord );

  m_menuFile->addSeparator();

  m_actionMinimize = hideWindowAction ( this, true );
  m_menuFile->addAction ( m_actionMinimize );

  m_actionQuit = quitAction ( this, true );
  m_menuFile->addAction ( m_actionQuit );

  // Actions
  m_menuActions = addMenu ( trUtf8 ( "Actions" ) );

  m_actionExport = exportAction ( this, true );
  m_menuActions->addAction ( m_actionExport );

  QMenu* m_menuBookmark = m_menuActions->addMenu ( QIcon::fromTheme ( "folder-bookmark" ),
                          trUtf8 ( "Bookmark" ) );

  m_actionAddBookmark = bookmarkCreateAction ( m_menuBookmark );
  m_menuBookmark->addAction ( m_actionAddBookmark );

  m_actionDelBookmark = bookmarkRemoveAction ( m_menuBookmark );
  m_menuBookmark->addAction ( m_actionDelBookmark );

  m_actionSave = saveAction ( this, true );
  m_menuActions->addAction ( m_actionSave );

  m_actionLoad = m_menuActions->addAction ( trUtf8 ( "Load" ) );
  m_actionLoad->setIcon ( QIcon::fromTheme ( "edit-redo" ) );
  m_actionLoad->setShortcut ( QKeySequence::Undo );

  m_actionRefresh = refreshAction ( this, true );
  m_menuActions->addAction ( m_actionRefresh );

  // About and Help
  QMenu* m_menuHelp = addMenu ( trUtf8 ( "Help" ) );

  QIcon helpIcon = QIcon::fromTheme ( "help-contents" );

  QAction* m_actionHJCMS = m_menuHelp->addAction ( getThemeIcon ( "qx11grab" ),
                           trUtf8 ( "QX11Grab" ) );
  m_actionHJCMS->setToolTip ( trUtf8 ( "QX11Grab Homepage" ) );

  QAction* m_actionFFmpeg = m_menuHelp->addAction ( helpIcon, trUtf8 ( "FFmpeg" ) );
  m_actionFFmpeg->setToolTip ( trUtf8 ( "FFmpeg Homepage" ) );

  QAction* m_actionx264 = m_menuHelp->addAction ( helpIcon, trUtf8 ( "x264" ) );
  m_actionx264->setToolTip ( trUtf8 ( "x264 Homepage" ) );

  QAction* m_actionWebM = m_menuHelp->addAction ( helpIcon, trUtf8 ( "WebM Guideline" ) );
  m_actionWebM->setToolTip ( trUtf8 ( "WebM Container Guidelines" ) );

  connect ( m_actionGrabbing, SIGNAL ( triggered () ),
            mainWindow, SLOT ( grabFromWindow () ) );

  connect ( m_actionStartRecord, SIGNAL ( triggered () ),
            mainWindow, SLOT ( startRecord () ) );

  connect ( m_actionStopRecord, SIGNAL ( triggered () ),
            mainWindow, SIGNAL ( stopRecording () ) );

  connect ( m_actionKillRecord, SIGNAL ( triggered () ),
            mainWindow, SIGNAL ( killProcess () ) );

  connect ( m_actionMinimize, SIGNAL ( triggered() ),
            mainWindow, SLOT ( hide() ) );

  connect ( m_actionQuit, SIGNAL ( triggered() ),
            qApp, SLOT ( quit() ) );

  connect ( m_actionExport, SIGNAL ( triggered() ),
            mainWindow, SLOT ( exportCommand() ) );

  connect ( m_actionAddBookmark, SIGNAL ( triggered() ),
            mainWindow, SLOT ( openCreateBookmark() ) );

  connect ( m_actionDelBookmark, SIGNAL ( triggered() ),
            mainWindow, SLOT ( openRemoveBookmark() ) );

  connect ( m_actionSave, SIGNAL ( triggered() ),
            mainWindow, SLOT ( saveSettings() ) );

  connect ( m_actionLoad, SIGNAL ( triggered() ),
            mainWindow, SLOT ( loadSettings() ) );

  connect ( m_actionRefresh, SIGNAL ( triggered() ),
            mainWindow, SLOT ( perparePreview() ) );

  // Homepages
  connect ( m_actionFFmpeg, SIGNAL ( triggered () ),
            this, SLOT ( openFFmpegHomepage() ) );

  connect ( m_actionHJCMS, SIGNAL ( triggered () ),
            this, SLOT ( openQX11GrabHomepage() ) );

  connect ( m_actionWebM, SIGNAL ( triggered () ),
            this, SLOT ( openWebMProjectHomepage() ) );

  connect ( m_actionx264, SIGNAL ( triggered () ),
            this, SLOT ( openVideoLanHomepage() ) );
}

QAction* MenuBar::bookmarkCreateAction ( QMenu * parent )
{
  QAction* ac = new QAction ( trUtf8 ( "Create" ), parent );
  ac->setObjectName ( QLatin1String ( "bookmarkCreateAction" ) );
  ac->setStatusTip ( trUtf8 ( "Open create Bookmark Dialog" ) );
  ac->setIcon ( QIcon::fromTheme ( "bookmark-new" ) );
  ac->setShortcut ( Qt::CTRL + Qt::Key_B );
  return ac;
}

QAction* MenuBar::bookmarkRemoveAction ( QMenu * parent )
{
  QAction* ac = new QAction ( trUtf8 ( "Remove" ), parent );
  ac->setObjectName ( QLatin1String ( "bookmarkRemoveAction" ) );
  ac->setStatusTip ( trUtf8 ( "Open delete Bookmark Dialog" ) );
  ac->setIcon ( QIcon::fromTheme ( "bookmarks-organize" ) );
  return ac;
}

void MenuBar::openFFmpegHomepage()
{
  QDesktopServices::openUrl ( QUrl ( "http://www.ffmpeg.org/documentation.html" ) );
}

void MenuBar::openQX11GrabHomepage()
{
  QDesktopServices::openUrl ( QUrl ( "http://qx11grab.hjcms.de" ) );
}

void MenuBar::openWebMProjectHomepage()
{
  QDesktopServices::openUrl ( QUrl ( "http://www.webmproject.org/code/specs/container/" ) );
}

void MenuBar::openVideoLanHomepage()
{
  QDesktopServices::openUrl ( QUrl ( "http://www.videolan.org/developers/x264.html" ) );
}

void MenuBar::setActionsEnabled ( bool b )
{
  m_actionStopRecord->setEnabled ( b );
  m_actionKillRecord->setEnabled ( b );
  m_actionStartRecord->setEnabled ( ( ( b ) ? false : true ) );
}

QAction* MenuBar::quitAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Quit" ), parent );
  ac->setObjectName ( QLatin1String ( "actionQuit" ) );
  ac->setStatusTip ( trUtf8 ( "Close Application" ) );
  ac->setIcon ( QIcon::fromTheme ( "application-exit" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Quit );
  return ac;
}

QAction* MenuBar::grabbingAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Grabbing" ), parent );
  ac->setObjectName ( QLatin1String ( "actionGrabbing" ) );
  ac->setStatusTip ( trUtf8 ( "get Size from Window" ) );
  ac->setIcon ( QIcon::fromTheme ( "window" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_G );
  return ac;
}

QAction* MenuBar::rubberbandAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Rubberband" ), parent );
  ac->setObjectName ( QLatin1String ( "actionRubberband" ) );
  ac->setStatusTip ( trUtf8 ( "get Size from Window" ) );
  ac->setIcon ( QIcon::fromTheme ( "view-grid" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Find );
  return ac;
}

QAction* MenuBar::startRecordAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Recording" ), parent );
  ac->setObjectName ( QLatin1String ( "actionStartRecord" ) );
  ac->setStatusTip ( trUtf8 ( "Start Recording" ) );
  ac->setIcon ( QIcon::fromTheme ( "media-record" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_R );
  return ac;
}

QAction* MenuBar::stopRecordAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Stop" ), parent );
  ac->setObjectName ( QLatin1String ( "actionStopRecord" ) );
  ac->setStatusTip ( trUtf8 ( "Stop Recording" ) );
  ac->setIcon ( QIcon::fromTheme ( "media-playback-stop" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_E );
  ac->setEnabled ( false );
  return ac;
}

QAction* MenuBar::killRecordAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Kill" ), parent );
  ac->setObjectName ( QLatin1String ( "actionKillRecord" ) );
  ac->setStatusTip ( trUtf8 ( "Kill Recording" ) );
  ac->setIcon ( QIcon::fromTheme ( "menu-debugger" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_Z );
  ac->setEnabled ( false );
  return ac;
}

QAction* MenuBar::hideWindowAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Hide" ), parent );
  ac->setObjectName ( QLatin1String ( "actionMinimize" ) );
  ac->setStatusTip ( trUtf8 ( "Minimize to Systray" ) );
  ac->setIcon ( getThemeIcon ( "minimize" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_H );
  return ac;
}

QAction* MenuBar::showWindowAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Show" ), parent );
  ac->setObjectName ( QLatin1String ( "actionMinimize" ) );
  ac->setStatusTip ( trUtf8 ( "Minimize to Systray" ) );
  ac->setIcon ( getThemeIcon ( "maximize" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_M );
  return ac;
}

QAction* MenuBar::exportAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Export" ), parent );
  ac->setObjectName ( QLatin1String ( "exportAction" ) );
  ac->setStatusTip ( trUtf8 ( "Export current command set" ) );
  ac->setIcon ( QIcon::fromTheme ( "document-export" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_N );
  return ac;
}

QAction* MenuBar::saveAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Save" ), parent );
  ac->setObjectName ( QLatin1String ( "actionSave" ) );
  ac->setStatusTip ( trUtf8 ( "Save current Settings" ) );
  ac->setIcon ( QIcon::fromTheme ( "document-save" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Save );
  return ac;
}

QAction* MenuBar::refreshAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Refresh" ), parent );
  ac->setObjectName ( QLatin1String ( "actionRefresh" ) );
  ac->setStatusTip ( trUtf8 ( "Refresh Commandline Preview" ) );
  ac->setIcon ( QIcon::fromTheme ( "view-refresh" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Refresh );
  return ac;
}

QAction* MenuBar::viewAction ( QObject * parent, bool shortcut )
{
  QAction* ac = new QAction ( trUtf8 ( "Logfile" ), parent );
  ac->setObjectName ( QLatin1String ( "actionLogfile" ) );
  ac->setStatusTip ( trUtf8 ( "Open FFmpeg Protocoll" ) );
  ac->setIcon ( QIcon::fromTheme ( "view-list-text" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Refresh );
  return ac;
}

MenuBar::~MenuBar()
{}

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
#include "menubar.h"
#include "aboutdialog.h"
#include "settings.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QProcessEnvironment>

/* QtGui */
#include <QtGui/QDesktopServices>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

MenuBar::MenuBar ( MainWindow * parent )
    : QMenuBar ( parent )
    , m_mainWindow ( parent )
{
  setObjectName ( QLatin1String ( "MenuBar" ) );
  m_menuFile = addMenu ( trUtf8 ( "File" ) );

  m_actionGrabbing = grabbingAction ( this, true );
  m_menuFile->addAction ( m_actionGrabbing );

  m_actionRubber = rubberbandAction ( this, true );
  m_menuFile->addAction ( m_actionRubber );

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

  QMenu* m_menuBookmark = m_menuActions->addMenu ( Settings::themeIcon ( "folder-bookmark" ),
                          trUtf8 ( "Bookmark" ) );

  m_actionAddBookmark = bookmarkCreateAction ( m_menuBookmark );
  m_menuBookmark->addAction ( m_actionAddBookmark );

  m_actionDelBookmark = bookmarkRemoveAction ( m_menuBookmark );
  m_menuBookmark->addAction ( m_actionDelBookmark );

  m_menuActions->addSeparator();

  m_actionSave = saveAction ( this, true );
  m_menuActions->addAction ( m_actionSave );

  m_actionLoad = m_menuActions->addAction ( trUtf8 ( "Load" ) );
  m_actionLoad->setIcon ( Settings::themeIcon ( "edit-redo" ) );
  m_actionLoad->setShortcut ( QKeySequence::Undo );

  m_actionRefresh = refreshAction ( this, true );
  m_menuActions->addAction ( m_actionRefresh );

  m_menuActions->addSeparator();

  m_actionExport = exportAction ( this, true );
  m_menuActions->addAction ( m_actionExport );

  /*: MenuEntry */
  m_actionPrEditor = m_menuActions->addAction ( trUtf8 ( "Presets Editor" ) );
  /*: ToolTip */
  m_actionPrEditor->setStatusTip ( trUtf8 ( "Open Presets Editor" ) );
  m_actionPrEditor->setIcon ( Settings::themeIcon ( "document-edit", "menu-editors" ) );

  m_menuActions->addSeparator();

  QAction* m_actionConfigure = m_menuActions->addAction ( trUtf8 ( "Configure" ) );
  m_actionConfigure->setIcon ( Settings::themeIcon ( "configure" ) );

  // About and Help
  QMenu* m_menuHelp = addMenu ( trUtf8 ( "Help" ) );

  QIcon helpIcon = Settings::themeIcon ( "help-contents" );

  QAction* m_actionHJCMS = m_menuHelp->addAction ( Settings::themeIcon ( "qx11grab" ),
                           trUtf8 ( "QX11Grab" ) );
  /*: ToolTip */
  m_actionHJCMS->setStatusTip ( trUtf8 ( "QX11Grab Homepage" ) );

  QAction* m_actionFFmpeg = m_menuHelp->addAction ( helpIcon, trUtf8 ( "FFmpeg" ) );
  /*: ToolTip */
  m_actionFFmpeg->setStatusTip ( trUtf8 ( "FFmpeg Homepage" ) );

  QAction* m_actionx264 = m_menuHelp->addAction ( helpIcon, trUtf8 ( "x264" ) );
  /*: ToolTip */
  m_actionx264->setStatusTip ( trUtf8 ( "x264 Homepage" ) );

  QAction* m_actionWebM = m_menuHelp->addAction ( helpIcon, trUtf8 ( "WebM Guideline" ) );
  /*: ToolTip */
  m_actionWebM->setStatusTip ( trUtf8 ( "WebM Container Guidelines" ) );

  // About
  m_menuHelp->addSeparator();
  QAction* m_aboutQX11Grab = m_menuHelp->addAction ( Settings::themeIcon ( "qx11grab" ),
                             trUtf8 ( "About QX11Grab" ) );

  QAction* m_aboutQt4 = m_menuHelp->addAction ( helpIcon, trUtf8 ( "About Qt4" ) );

  // SIGNALS
  connect ( m_actionGrabbing, SIGNAL ( triggered () ),
            m_mainWindow, SLOT ( grabFromWindow () ) );

  connect ( m_actionRubber, SIGNAL ( triggered () ),
            m_mainWindow, SLOT ( swapRubberBand () ) );

  connect ( m_actionStartRecord, SIGNAL ( triggered () ),
            m_mainWindow, SLOT ( startRecord () ) );

  connect ( m_actionStopRecord, SIGNAL ( triggered () ),
            m_mainWindow, SIGNAL ( stopRecording () ) );

  connect ( m_actionKillRecord, SIGNAL ( triggered () ),
            m_mainWindow, SIGNAL ( killProcess () ) );

  connect ( m_actionMinimize, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( hide() ) );

  connect ( m_actionQuit, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( shutdown() ) );

  connect ( m_actionExport, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( exportCommand() ) );

  connect ( m_actionAddBookmark, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( openCreateBookmark() ) );

  connect ( m_actionDelBookmark, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( openRemoveBookmark() ) );

  connect ( m_actionPrEditor, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( openPresetEditor() ) );

  connect ( m_actionSave, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( saveSettings() ) );

  connect ( m_actionLoad, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( loadSettings() ) );

  connect ( m_actionRefresh, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( preparePreview() ) );

  connect ( m_actionConfigure, SIGNAL ( triggered() ),
            m_mainWindow, SLOT ( openConfiguration() ) );

  // Homepages
  connect ( m_actionFFmpeg, SIGNAL ( triggered () ),
            this, SLOT ( openFFmpegHomepage() ) );

  connect ( m_actionHJCMS, SIGNAL ( triggered () ),
            this, SLOT ( openQX11GrabHomepage() ) );

  connect ( m_actionWebM, SIGNAL ( triggered () ),
            this, SLOT ( openWebMProjectHomepage() ) );

  connect ( m_actionx264, SIGNAL ( triggered () ),
            this, SLOT ( openVideoLanHomepage() ) );

  // about
  connect ( m_aboutQX11Grab, SIGNAL ( triggered () ),
            this, SLOT ( openAboutDialog() ) );

  connect ( m_aboutQt4, SIGNAL ( triggered () ),
            qApp, SLOT ( aboutQt() ) );
}

void MenuBar::openWebLink ( const QUrl &u )
{
  QByteArray ch = qgetenv ( "BROWSER" );
  if ( ! ch.isEmpty() )
  {
    QString browser ( ch );
    if ( browser.contains ( "firefox" ) || browser.contains ( "chrom" ) )
    {
      QProcess process;
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      process.setProcessEnvironment ( env );
      process.startDetached ( browser, QStringList ( u.toString() ) );
    }
    return;
  }
  QDesktopServices::openUrl ( u );
}

QAction* MenuBar::bookmarkCreateAction ( QMenu * parent )
{
  QAction* ac = new QAction ( trUtf8 ( "Create" ), parent );
  ac->setObjectName ( QLatin1String ( "bookmarkCreateAction" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Open create Bookmark Dialog" ) );
  ac->setIcon ( Settings::themeIcon ( "bookmark-new" ) );
  ac->setShortcut ( Qt::CTRL + Qt::Key_B );
  return ac;
}

QAction* MenuBar::bookmarkRemoveAction ( QMenu * parent )
{
  QAction* ac = new QAction ( trUtf8 ( "Remove" ), parent );
  ac->setObjectName ( QLatin1String ( "bookmarkRemoveAction" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Open delete Bookmark Dialog" ) );
  ac->setIcon ( Settings::themeIcon ( "bookmarks-organize" ) );
  return ac;
}

void MenuBar::openFFmpegHomepage()
{
  openWebLink ( QUrl ( "http://www.ffmpeg.org/documentation.html" ) );
}

void MenuBar::openQX11GrabHomepage()
{
  openWebLink ( QUrl ( "http://qx11grab.hjcms.de" ) );
}

void MenuBar::openWebMProjectHomepage()
{
  openWebLink ( QUrl ( "http://www.webmproject.org/code/specs/container/" ) );
}

void MenuBar::openVideoLanHomepage()
{
  openWebLink ( QUrl ( "http://www.videolan.org/developers/x264.html" ) );
}

void MenuBar::openAboutDialog()
{
  AboutDialog dialog ( this );
  dialog.exec();
}

void MenuBar::setActionsEnabled ( bool b )
{
  m_actionStopRecord->setEnabled ( b );
  m_actionKillRecord->setEnabled ( b );
  m_actionStartRecord->setEnabled ( ( ( b ) ? false : true ) );
}

QAction* MenuBar::quitAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Quit" ), parent );
  ac->setObjectName ( QLatin1String ( "actionQuit" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Close Application" ) );
  ac->setIcon ( Settings::themeIcon ( "application-exit" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Quit );
  return ac;
}

QAction* MenuBar::grabbingAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Grabbing" ), parent );
  ac->setObjectName ( QLatin1String ( "actionGrabbing" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "get Size from Window" ) );
  ac->setIcon ( Settings::themeIcon ( "window-duplicate" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_G );
  return ac;
}

QAction* MenuBar::rubberbandAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Rubberband" ), parent );
  ac->setObjectName ( QLatin1String ( "actionRubberband" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "swap rubberband view" ) );
  ac->setIcon ( Settings::themeIcon ( "view-grid" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::SHIFT + Qt::Key_R );
  return ac;
}

QAction* MenuBar::startRecordAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Recording" ), parent );
  ac->setObjectName ( QLatin1String ( "actionStartRecord" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Start Recording" ) );
  ac->setIcon ( Settings::themeIcon ( "media-record" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_R );
  return ac;
}

QAction* MenuBar::stopRecordAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Stop" ), parent );
  ac->setObjectName ( QLatin1String ( "actionStopRecord" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Stop Recording" ) );
  ac->setIcon ( Settings::themeIcon ( "media-playback-stop" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_E );
  ac->setEnabled ( false );
  return ac;
}

QAction* MenuBar::killRecordAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Kill" ), parent );
  ac->setObjectName ( QLatin1String ( "actionKillRecord" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Kill Recording" ) );
  ac->setIcon ( Settings::themeIcon ( "menu-debugger", "dialog-cancel" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_Z );
  ac->setEnabled ( false );
  return ac;
}

QAction* MenuBar::hideWindowAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Hide" ), parent );
  ac->setObjectName ( QLatin1String ( "actionMinimize" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Minimize to Systray" ) );
  ac->setIcon ( Settings::themeIcon ( "minimize" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_H );
  return ac;
}

QAction* MenuBar::showWindowAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Show" ), parent );
  ac->setObjectName ( QLatin1String ( "actionMinimize" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Minimize to Systray" ) );
  ac->setIcon ( Settings::themeIcon ( "maximize" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_M );
  return ac;
}

QAction* MenuBar::exportAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Export" ), parent );
  ac->setObjectName ( QLatin1String ( "exportAction" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Export current command set" ) );
  ac->setIcon ( Settings::themeIcon ( "document-export" ) );
  if ( shortcut )
    ac->setShortcut ( Qt::CTRL + Qt::Key_N );
  return ac;
}

QAction* MenuBar::saveAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Save" ), parent );
  ac->setObjectName ( QLatin1String ( "actionSave" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Save current Settings" ) );
  ac->setIcon ( Settings::themeIcon ( "document-save" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Save );
  return ac;
}

QAction* MenuBar::refreshAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Refresh" ), parent );
  ac->setObjectName ( QLatin1String ( "actionRefresh" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Refresh Commandline Preview" ) );
  ac->setIcon ( Settings::themeIcon ( "view-refresh" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Refresh );
  return ac;
}

QAction* MenuBar::viewAction ( QObject * parent, bool shortcut )
{
  /*: MenuEntry */
  QAction* ac = new QAction ( trUtf8 ( "Logfile" ), parent );
  ac->setObjectName ( QLatin1String ( "actionLogfile" ) );
  /*: ToolTip */
  ac->setStatusTip ( trUtf8 ( "Open FFmpeg Protocoll" ) );
  ac->setIcon ( Settings::themeIcon ( "view-list-text" ) );
  if ( shortcut )
    ac->setShortcut ( QKeySequence::Refresh );
  return ac;
}

MenuBar::~MenuBar()
{}

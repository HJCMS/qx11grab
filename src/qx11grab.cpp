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

#include "qx11grab.h"
#include "settings.h"
#include "desktopinfo.h"
#include "rubberband.h"
#include "windowgrabber.h"
#include "grabberinfo.h"
#include "defaults.h"
#include "tableeditor.h"
#include "metadata.h"
#include "ffprocess.h"
#include "commandpreview.h"
#include "qx11grabadaptor.h"
#include "logviewer.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QRubberBand>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QX11Info>

/* QtDBus */
#include <QtDBus/QDBusConnection>

QX11Grab::QX11Grab ( Settings *settings )
    : QMainWindow(), cfg ( settings )
{
  setupUi ( this );
  setMinimumWidth ( 500 );
  setMinimumHeight ( 450 );

  QIcon boxIcon;
  boxIcon.addFile ( QString::fromUtf8 ( "://images/qx11grab.png" ), QSize(), QIcon::Normal, QIcon::Off );

  QWidget* layerWidget = new QWidget ( this );

  QVBoxLayout* verticalLayout = new QVBoxLayout ( layerWidget );

  m_splitter = new QSplitter ( Qt::Vertical, layerWidget );
  m_splitter->setObjectName ( QLatin1String ( "splitter" ) );
  verticalLayout->addWidget ( m_splitter );

  QToolBox* toolBox = new QToolBox ( m_splitter, Qt::Widget );
  toolBox->setObjectName ( QLatin1String ( "toolbox" ) );
  m_splitter->insertWidget ( 0, toolBox );

  m_grabberInfo = new GrabberInfo ( toolBox );
  toolBox->addItem ( m_grabberInfo, boxIcon, QString::fromUtf8 ( "QX11Grab" ) );

  m_defaults = new Defaults ( toolBox );
  toolBox->addItem ( m_defaults, boxIcon, QString::fromUtf8 ( "Default" ) );

  m_metaData = new MetaData ( toolBox );
  m_metaData->setToolTip ( QString::fromUtf8 ( "-metadata" ) );
  toolBox->addItem ( m_metaData, boxIcon, QString::fromUtf8 ( "Metadata" ) );

  m_videoEditor = new TableEditor ( toolBox );
  m_videoEditor->setToolTip ( QString::fromUtf8 ( "-vcodec" ) );
  toolBox->addItem ( m_videoEditor, boxIcon, QString::fromUtf8 ( "Video" ) );

  m_audioEditor = new TableEditor ( toolBox );
  m_audioEditor->setToolTip ( QString::fromUtf8 ( "-acodec" ) );
  toolBox->addItem ( m_audioEditor, boxIcon, QString::fromUtf8 ( "Audio" ) );

  m_commandPreview = new CommandPreview ( m_splitter );
  m_splitter->insertWidget ( 1, m_commandPreview );

  QWidget* layerActionsWidget = new QWidget ( layerWidget );
  layerActionsWidget->setObjectName ( QLatin1String ( "layeractionswidget" ) );

  QHBoxLayout* horizontalLayout = new QHBoxLayout ( layerActionsWidget );
  horizontalLayout->setContentsMargins ( 0, 2, 0, 2 );

  QPushButton* rerfreshBtn = new QPushButton ( layerActionsWidget );
  rerfreshBtn->setText ( trUtf8 ( "Refresh" ) );
  rerfreshBtn->setIcon ( getThemeIcon ( "view-refresh" ) );
  horizontalLayout->addWidget ( rerfreshBtn );

  logviewBtn = new QPushButton ( layerActionsWidget );
  logviewBtn->setText ( trUtf8 ( "Logfile" ) );
  logviewBtn->setIcon ( getThemeIcon ( "view-list-text" ) );
  logviewBtn->setEnabled ( false );
  horizontalLayout->addWidget ( logviewBtn );

  horizontalLayout->addStretch ( 1 );

  QPushButton* saveBtn = new QPushButton ( layerActionsWidget );
  saveBtn->setText ( trUtf8 ( "Save" ) );
  saveBtn->setIcon ( getThemeIcon ( "document-save" ) );
  horizontalLayout->addWidget ( saveBtn );

  layerActionsWidget->setLayout ( horizontalLayout );
  verticalLayout->addWidget ( layerActionsWidget );

  layerWidget->setLayout ( verticalLayout );
  setCentralWidget ( layerWidget );

  TimeOutMessages = 5000;

  loadStats();

  /* NOTE ffprocess must init before create Actions ... */
  m_FFProcess = new FFProcess ( this, cfg );

  createActions();
  createEnviroment();
  createSystemTrayIcon();

  m_busAdaptor = new QX11GrabAdaptor ( this );
  connect ( m_FFProcess, SIGNAL ( message ( const QString & ) ),
            m_busAdaptor, SIGNAL ( message ( const QString & ) ) );

  /* Signals */
  connect ( m_FFProcess, SIGNAL ( message ( const QString & ) ),
            this, SLOT ( pushInfoMessage ( const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( errmessage ( const QString &, const QString & ) ),
            this, SLOT ( pushErrorMessage ( const QString &, const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( trigger ( const QString & ) ),
            this, SLOT ( pushToolTip ( const QString & ) ) );

  connect ( m_grabberInfo, SIGNAL ( screenDataChanged ( int ) ),
            this, SLOT ( toRubber ( int ) ) );

  connect ( m_grabberInfo, SIGNAL ( showRubber ( bool ) ),
            this, SLOT ( showRubber ( bool ) ) );

  connect ( m_grabberInfo, SIGNAL ( postUpdate () ),
            this, SLOT ( perparePreview () ) );

  connect ( m_videoEditor, SIGNAL ( postUpdate () ),
            this, SLOT ( perparePreview () ) );

  connect ( m_audioEditor, SIGNAL ( postUpdate () ),
            this, SLOT ( perparePreview () ) );

  connect ( actionGrabbing, SIGNAL ( triggered () ),
            this, SLOT ( grabFromWindow () ) );

  connect ( actionStartRecord, SIGNAL ( triggered () ),
            this, SLOT ( startRecord () ) );

  connect ( actionStopRecord, SIGNAL ( triggered () ),
            m_FFProcess, SLOT ( stop () ) );

  connect ( actionKillRecord, SIGNAL ( triggered () ),
            m_FFProcess, SLOT ( kill () ) );

  connect ( m_FFProcess, SIGNAL ( down () ),
            this, SLOT ( setActionsBack () ) );

  connect ( m_commandPreview, SIGNAL ( dataSaved ( const QStringList & ) ),
            this, SLOT ( updateCommandLine ( const QStringList & ) ) );

  connect ( actionMinimize, SIGNAL ( triggered() ),
            this, SLOT ( hide() ) );

  connect ( actionQuit, SIGNAL ( triggered() ),
            qApp, SLOT ( quit() ) );

  connect ( actionSave, SIGNAL ( triggered() ),
            this, SLOT ( saveSettings() ) );

  connect ( actionLoad, SIGNAL ( triggered() ),
            this, SLOT ( loadSettings() ) );

  connect ( actionRefresh, SIGNAL ( triggered() ),
            this, SLOT ( perparePreview() ) );

  connect ( rerfreshBtn, SIGNAL ( clicked() ),
            this, SLOT ( perparePreview() ) );

  connect ( logviewBtn, SIGNAL ( clicked() ),
            this, SLOT ( openLogFileDialog() ) );

  connect ( saveBtn, SIGNAL ( clicked() ),
            this, SLOT ( saveSettings() ) );

}

void QX11Grab::record()
{
  if ( ! m_FFProcess )
    return;

#ifdef QT_NO_EXCEPTIONS

  startRecord ();

#else

  try
  {
    startRecord ();
  }
  catch ( const char* mess )
  {
    qWarning ( "No XServer where found: (%s)", mess );
  }

#endif
}

void QX11Grab::stop()
{
  if ( m_FFProcess->isRunning() )
    m_FFProcess->stop ();
}

void QX11Grab::createActions()
{
  grabActionFromWindow = new QAction ( getThemeIcon ( "window" ), trUtf8 ( "Grabbing" ), this );
  connect ( grabActionFromWindow, SIGNAL ( triggered() ), this, SLOT ( grabFromWindow() ) );

  showRubberbandWindow = new QAction ( getThemeIcon ( "view-grid" ), trUtf8 ( "Rubberband" ), this );
  connect ( showRubberbandWindow, SIGNAL ( triggered() ), this, SLOT ( swapRubberBand() ) );

  startRecordingWindow = new QAction ( getThemeIcon ( "media-record" ), trUtf8 ( "Recording" ), this );
  connect ( startRecordingWindow, SIGNAL ( triggered() ), this, SLOT ( startRecord() ) );

  stopRecordingWindow = new QAction ( getThemeIcon ( "media-playback-stop" ), trUtf8 ( "Stop" ), this );
  stopRecordingWindow->setEnabled ( false );
  connect ( stopRecordingWindow, SIGNAL ( triggered() ), m_FFProcess, SLOT ( stop () ) );

  minimizeWindowAction = new QAction ( getThemeIcon ( "minimize" ), trUtf8 ( "Hide" ), this );
  connect ( minimizeWindowAction, SIGNAL ( triggered() ), this, SLOT ( hide() ) );

  displayWindowAction = new QAction ( getThemeIcon ( "maximize" ), trUtf8 ( "Show" ), this );
  connect ( displayWindowAction, SIGNAL ( triggered() ), this, SLOT ( showNormal() ) );

  quitWindowAction = new QAction ( getThemeIcon ( "application-exit" ), trUtf8 ( "Quit" ), this );
  connect ( quitWindowAction, SIGNAL ( triggered() ), qApp, SLOT ( quit() ) );

  /* Window Icons */
  actionGrabbing->setIcon ( getThemeIcon ( "window" ) );
  actionGrabbing->setShortcut ( Qt::CTRL + Qt::Key_G );

  actionStartRecord->setIcon ( getThemeIcon ( "media-record" ) );
  actionStartRecord->setShortcut ( Qt::CTRL + Qt::Key_R );

  actionStopRecord->setIcon ( getThemeIcon ( "media-playback-stop" ) );
  actionStopRecord->setShortcut ( Qt::CTRL + Qt::Key_E );

  actionKillRecord->setIcon ( getThemeIcon ( "window-close" ) );
  actionKillRecord->setShortcut ( Qt::CTRL + Qt::Key_Z );

  actionMinimize->setIcon ( getThemeIcon ( "minimize" ) );
  actionMinimize->setShortcut ( Qt::CTRL + Qt::Key_H );

  actionQuit->setIcon ( getThemeIcon ( "application-exit" ) );
  actionQuit->setShortcut ( QKeySequence::Quit );

  actionSave->setIcon ( getThemeIcon ( "document-save" ) );
  actionSave->setShortcut ( QKeySequence::Save );

  actionLoad->setIcon ( getThemeIcon ( "edit-redo" ) );
  actionLoad->setShortcut ( QKeySequence::Undo );

  actionRefresh->setIcon ( getThemeIcon ( "view-refresh" ) );
  actionRefresh->setShortcut ( QKeySequence::Refresh );
}

void QX11Grab::createEnviroment()
{
  m_DesktopInfo = new DesktopInfo ( this );
  FrameMode desktop = m_DesktopInfo->grabScreenGeometry ( centralWidget() );
//  setDepthBox->setValue ( desktop.depth );

  m_RubberBand = new RubberBand ( qApp->desktop()->screen() );
  connect ( m_RubberBand, SIGNAL ( error ( const QString &, const QString & ) ),
            this, SLOT ( pushErrorMessage ( const QString &, const QString & ) ) );

  toRubber ( 1 );
  if ( m_grabberInfo->RubberbandIsVisible() )
    m_RubberBand->show();
  else
    m_RubberBand->hide();
}

void QX11Grab::createSystemTrayIcon()
{
#ifdef HAVE_KDE4_SUPPORT
  systemTrayMenu = new KMenu ( this );
#else
  systemTrayMenu = new QMenu ( this );
#endif
  systemTrayMenu->addAction ( grabActionFromWindow );
  systemTrayMenu->addAction ( showRubberbandWindow );
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction ( startRecordingWindow );
  systemTrayMenu->addAction ( stopRecordingWindow );
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction ( minimizeWindowAction );
  systemTrayMenu->addAction ( displayWindowAction );
  systemTrayMenu->addSeparator();
  systemTrayMenu->addAction ( quitWindowAction );

#ifdef HAVE_KDE4_SUPPORT
  systemTrayIcon = new KSystemTrayIcon ( this );
#else
  systemTrayIcon = new QSystemTrayIcon ( this );
  connect ( systemTrayIcon, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ),
            this, SLOT ( systemTrayWatcher ( QSystemTrayIcon::ActivationReason ) ) );
#endif
  systemTrayIcon->setIcon ( getThemeIcon ( "qx11grab" ) );
  systemTrayIcon->setToolTip ( trUtf8 ( "qx11grab: recording X11 Windows with ffmpeg" ) );
  systemTrayIcon->setContextMenu ( systemTrayMenu );
  systemTrayIcon->show();
}

void QX11Grab::swapRubberBand ()
{
  if ( m_grabberInfo->RubberbandIsVisible() )
  {
    m_grabberInfo->setRubberbandCheckBox ( false );
    m_RubberBand->hide();
  }
  else
  {
    m_grabberInfo->setRubberbandCheckBox ( true );
    m_RubberBand->show();
  }
}

/**
* Ein/Ausblenden funktion für die Gummibandanzeige.
*/
void QX11Grab::showRubber ( bool b )
{
  if ( b )
  {
    m_grabberInfo->setRubberbandCheckBox ( true );
    m_RubberBand->show();
  }
  else
  {
    m_grabberInfo->setRubberbandCheckBox ( false );
    m_RubberBand->hide();
  }
}

/**
* Lese die Fenster Geometrien neu ein.
*/
void QX11Grab::loadStats()
{
  if ( cfg->contains ( "window/position" ) )
    move ( cfg->value ( "window/position", pos() ).toPoint() );

  if ( cfg->contains ( "window/size" ) )
    resize ( cfg->value ( "window/size", size() ).toSize() );

  loadSettings();

  QFileInfo log ( qx11grabLogfile() );
  logviewBtn->setEnabled ( log.exists() );
}

/**
* Fenster Verhältnisse Speichern
*/
void QX11Grab::saveStats()
{
  cfg->setValue ( "window/state", saveState() );
  cfg->setValue ( "window/position", pos() );
  cfg->setValue ( "window/size", size() );
}

/**
* Sendet schiebe Informationen an die Klasse @class RubberBand
*/
void QX11Grab::toRubber ( int i )
{
  if ( i < 1 )
    return;

  if ( ! m_RubberBand )
    return;

  QRect r = m_grabberInfo->getRect();
  m_RubberBand->resize ( r.width(), r.height() );
  m_RubberBand->move ( r.x(), r.y() );
  perparePreview();
}

/**
* Fenster Dimensionen abgreifen
*/
void QX11Grab::grabFromWindow()
{
  if ( ! m_RubberBand )
    return;

  WindowGrabber* grabber = new WindowGrabber ( this );
  QRect rect = grabber->grabWindowRect();

  if ( rect.isValid() )
  {
    m_grabberInfo->setScreenWidth ( rect.width() );
    m_grabberInfo->setScreenHeight ( rect.height() );
    m_grabberInfo->setScreenX ( rect.x() );
    m_grabberInfo->setScreenY ( rect.y() );
    m_grabberInfo->setScreenMode ( trUtf8 ( "grabbed Dimension" ) );
    toRubber ( 1 );
  }

  delete grabber;
}

/**
* Statusleisten Aktionen verarbeiten
*/
void QX11Grab::systemTrayWatcher ( QSystemTrayIcon::ActivationReason type )
{
  switch ( type )
  {
    case QSystemTrayIcon::Unknown:
      return;

    case QSystemTrayIcon::Trigger:
    {
      if ( isVisible() )
        hide();
      else
        show();
    }
    break;

    default:
      return;
  }
}

/**
* Beim Maximieren die Fenster Geometrie laden
*/
void QX11Grab::showEvent ( QShowEvent * )
{
  loadStats();
}

/**
* Beim Minimieren die Fenster Geometrie speichern
*/
void QX11Grab::hideEvent ( QHideEvent * )
{
  saveStats();
}

/**
* Vor dem beenden einige prüfungen durchführen
*/
void QX11Grab::closeEvent ( QCloseEvent *ev )
{
  if ( m_FFProcess->isRunning() )
  {
    QMessageBox::warning ( this, trUtf8 ( "Warning" ), trUtf8 ( "Recorder is running." ) );
    ev->ignore();
    m_FFProcess->deleteLater ();
  }
  saveStats();

  if ( m_busAdaptor )
  {
    QDBusConnection::sessionBus().unregisterObject ( "/qx11grab", QDBusConnection::UnregisterNode );
    delete m_busAdaptor;
  }
}

/**
* Informationen an die Statusleiste senden.
*/
void QX11Grab::pushInfoMessage ( const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->showMessage ( trUtf8 ( "Info" ), txt,
                                  QSystemTrayIcon::Information, TimeOutMessages );

  if ( ! m_FFProcess->isRunning() )
    systemTrayIcon->setIcon ( getThemeIcon ( "qx11grab" ) );
}

/**
* Fehler Meldungen an die Statusleiste senden.
*/
void QX11Grab::pushErrorMessage ( const QString &title, const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->showMessage ( title, txt, QSystemTrayIcon::Critical, TimeOutMessages );

  if ( ! m_FFProcess->isRunning() )
    systemTrayIcon->setIcon ( getThemeIcon ( "qx11grab" ) );
}

/**
* Tips an die Statusleiste senden.
*/
void QX11Grab::pushToolTip ( const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->setToolTip ( txt );
}

/**
* Die Daten wurden von @class CommandLineEdit modifiziert
* und müssen neu geschrieben werden.
**/
void QX11Grab::updateCommandLine ( const QStringList &cmd )
{
  if ( cmd.contains ( m_defaults->binary() ) )
    cfg->setValue ( QLatin1String ( "CurrentCommandLine" ), cmd );
}

/**
* Starte die Aufnahme und Sperre gleichzeitig
* einige Aktionen um doppel Klicks zu vermeiden.
*/
void QX11Grab::startRecord()
{
  if ( ! m_RubberBand->isScalability() )
    return;

  if ( m_FFProcess->create ( m_grabberInfo->getRect() ) )
  {
    stopRecordingWindow->setEnabled ( true );
    actionStopRecord->setEnabled ( true );
    actionKillRecord->setEnabled ( true );
    startRecordingWindow->setEnabled ( false );
    actionStartRecord->setEnabled ( false );
    showRubber ( false );
    QStringList cmd = cfg->value ( QLatin1String ( "CurrentCommandLine" ) ).toStringList();
    if ( m_FFProcess->start ( cmd ) )
    {
      systemTrayIcon->setIcon ( getThemeIcon ( "media-record" ) );
    }
  }
  else
    QMessageBox::critical ( this, trUtf8 ( "Error" ), trUtf8 ( "qx11grap not started" ) );

  logviewBtn->setEnabled ( true );
}

/**
* Beim beenden einer Aufnahme alles in die Neutrale Stellung bringen.
*/
void QX11Grab::setActionsBack()
{
  stopRecordingWindow->setEnabled ( false );
  actionStopRecord->setEnabled ( false );
  actionKillRecord->setEnabled ( false );
  startRecordingWindow->setEnabled ( true );
  actionStartRecord->setEnabled ( true );
  systemTrayIcon->setIcon ( getThemeIcon ( "qx11grab" ) );
}

/**
* Lade beim Start des Dialoges alle Einstellungen.
*/
void QX11Grab::loadSettings()
{
  m_grabberInfo->load ( cfg );
  m_defaults->load ( cfg );
  m_metaData->load ( cfg );
  m_videoEditor->load ( QString::fromUtf8 ( "VideoOptions" ), cfg );
  m_audioEditor->load ( QString::fromUtf8 ( "AudioOptions" ), cfg );
  perparePreview();
}

/**
* Speichere alle Einstellungen.
*/
void QX11Grab::saveSettings()
{
  m_grabberInfo->save ( cfg );
  m_defaults->save ( cfg );
  m_metaData->save ( cfg );
  m_videoEditor->save ( QString::fromUtf8 ( "VideoOptions" ), cfg );
  m_audioEditor->save ( QString::fromUtf8 ( "AudioOptions" ), cfg );
}

/**
* Log Dialog öffnen
*/
void QX11Grab::openLogFileDialog()
{
  QFileInfo log ( qx11grabLogfile() );
  if ( log.isReadable() )
  {
    LogViewer dialog ( log, centralWidget() );
    dialog.exec();
  }
}

/**
* Kommando Zeile für Textausgabe Aufbereiten.
*/
void QX11Grab::perparePreview()
{
  QStringList commandLine;

  commandLine << m_defaults->binary ();
  commandLine << "-f" << "x11grab" << "-xerror";

  QRect r = m_grabberInfo->getRect();
  commandLine << "-r" << QString::number ( m_grabberInfo->frameRate() );
  commandLine << "-s" << QString ( "%1x%2" ).arg (
      QString::number ( r.width() ), QString::number ( r.height() )
  );

  QX11Info xInfo;
  commandLine << "-i" << QString ( ":%1.%2+%3,%4 " ) .arg (
      QString::number ( xInfo.screen() ),
      QString::number ( xInfo.appScreen() ),
      QString::number ( r.x() ),
      QString::number ( r.y() )
  );

  // Video Options
  commandLine << m_videoEditor->getCmd ();

  // Meta Daten
  if ( m_grabberInfo->metaEnabled() )
    commandLine << m_metaData->getCmd ();

  // Audio Aufnahme
  if ( m_grabberInfo->soundEnabled() )
  {
    commandLine << m_defaults->audioDeviceData();
    commandLine << m_audioEditor->getCmd ();
  }

  // Output Options
  commandLine << "-y" << m_defaults->output();

  m_commandPreview->setCommandLine ( commandLine );

  cfg->setValue ( QLatin1String ( "CurrentCommandLine" ), commandLine );
}

/**
* Liest die aktuelle Kommandozeile aus der Konfiguration
*/
const QString QX11Grab::currentCommandLine()
{
  QStringList cmd = cfg->value ( QLatin1String ( "CurrentCommandLine" ) ).toStringList();
  return cmd.join ( " " );
}

/**
* Zeichenketten aus der Konfiguration lesen
*/
const QString QX11Grab::getSettingsValue ( const QString &key )
{
  return cfg->value ( key, "" ).toString();
}

QX11Grab::~QX11Grab()
{}

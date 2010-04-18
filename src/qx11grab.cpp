/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
* Copyright: See COPYING file that comes with this distribution
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

#ifdef HAVE_DBUS
#include "qx11grabadaptor.h"
#include <QtDBus/QDBusConnection>
#endif

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QDesktopWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>
#include <QtGui/QPushButton>
#include <QtGui/QRubberBand>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QX11Info>

QX11Grab::QX11Grab ( Settings *settings )
    : cfg ( settings )
{
  setupUi ( this );
  setMinimumWidth ( 500 );
  setMinimumHeight ( 450 );

  QIcon boxIcon;
  boxIcon.addFile ( QString::fromUtf8 ( ":/images/qx11grab.png" ), QSize(), QIcon::Normal, QIcon::Off );

  QWidget* layerWidget = new QWidget ( this );
  QVBoxLayout* verticalLayout = new QVBoxLayout ( layerWidget );

  QToolBox* toolBox = new QToolBox ( this, Qt::Widget );
  verticalLayout->addWidget ( toolBox );

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

  commandLineEdit = new QTextEdit ( this );
  commandLineEdit->setObjectName ( "CommandLine" );
  commandLineEdit->setToolTip ( trUtf8 ( "Display the current FFmpeg command." ) );
  verticalLayout->addWidget ( commandLineEdit );

  QWidget* layerActionsWidget = new QWidget ( this );

  QHBoxLayout* horizontalLayout = new QHBoxLayout ( layerActionsWidget );
  QPushButton* rerfreshBtn = new QPushButton ( layerActionsWidget );
  rerfreshBtn->setText ( trUtf8 ( "Refresh" ) );
  rerfreshBtn->setIcon ( getIcon ( "view-refresh" ) );
  horizontalLayout->addWidget ( rerfreshBtn );

  horizontalLayout->addStretch ( 1 );

  QPushButton* saveBtn = new QPushButton ( layerActionsWidget );
  saveBtn->setText ( trUtf8 ( "Save" ) );
  saveBtn->setIcon ( getIcon ( "document-save" ) );
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

#ifdef HAVE_DBUS

  m_QX11GrabAdaptor = new QX11GrabAdaptor ( this );
  connect ( m_FFProcess, SIGNAL ( message ( const QString & ) ),
            m_QX11GrabAdaptor, SIGNAL ( message ( const QString & ) ) );

#endif

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

  connect ( saveBtn, SIGNAL ( clicked() ),
            this, SLOT ( saveSettings() ) );

  connect ( rerfreshBtn, SIGNAL ( clicked() ),
            this, SLOT ( perparePreview() ) );
}

#ifdef HAVE_DBUS

void QX11Grab::start_record()
{
  if ( ! m_FFProcess )
    return;

  try
  {
    startRecord ();
  }
  catch ( const char* mess )
  {
    qFatal ( "No XServer where found: (%s)", mess );
  }
}

void QX11Grab::stop_record()
{
  if ( m_FFProcess->isRunning() )
    m_FFProcess->stop ();
}

#endif

const QIcon QX11Grab::getIcon ( const QString &name, const QString &group )
{
  QIcon icon;
  QString file = QString::fromUtf8 ( ":%1/images/%2.png" ).arg ( group, name );
  icon.addPixmap ( QPixmap ( file ), QIcon::Normal, QIcon::Off );

  return QIcon::fromTheme ( name, icon );
}

void QX11Grab::createActions()
{
  grabActionFromWindow = new QAction ( getIcon ( "window" ), trUtf8 ( "Grabbing" ), this );
  connect ( grabActionFromWindow, SIGNAL ( triggered() ), this, SLOT ( grabFromWindow() ) );

  showRubberbandWindow = new QAction ( getIcon ( "view-grid" ), trUtf8 ( "Rubberband" ), this );
  connect ( showRubberbandWindow, SIGNAL ( triggered() ), this, SLOT ( swapRubberBand() ) );

  startRecordingWindow = new QAction ( getIcon ( "run-build" ), trUtf8 ( "Recording" ), this );
  connect ( startRecordingWindow, SIGNAL ( triggered() ), this, SLOT ( startRecord() ) );

  stopRecordingWindow = new QAction ( getIcon ( "process-stop" ), trUtf8 ( "Stop" ), this );
  stopRecordingWindow->setEnabled ( false );
  connect ( stopRecordingWindow, SIGNAL ( triggered() ), m_FFProcess, SLOT ( stop () ) );

  minimizeWindowAction = new QAction ( getIcon ( "minimize" ), trUtf8 ( "Hide" ), this );
  connect ( minimizeWindowAction, SIGNAL ( triggered() ), this, SLOT ( hide() ) );

  displayWindowAction = new QAction ( getIcon ( "maximize" ), trUtf8 ( "Show" ), this );
  connect ( displayWindowAction, SIGNAL ( triggered() ), this, SLOT ( showNormal() ) );

  quitWindowAction = new QAction ( getIcon ( "application-exit" ), trUtf8 ( "Quit" ), this );
  connect ( quitWindowAction, SIGNAL ( triggered() ), qApp, SLOT ( quit() ) );

  /* Window Icons */
  actionGrabbing->setIcon ( getIcon ( "window" ) );
  actionGrabbing->setShortcut( Qt::CTRL + Qt::Key_G );

  actionStartRecord->setIcon ( getIcon ( "run-build" ) );
  actionStartRecord->setShortcut( Qt::CTRL + Qt::Key_R );

  actionStopRecord->setIcon ( getIcon ( "process-stop" ) );
  actionStopRecord->setShortcut( Qt::CTRL + Qt::Key_E );

  actionKillRecord->setIcon ( getIcon ( "window-close" ) );
  actionKillRecord->setShortcut( Qt::CTRL + Qt::Key_Z );

  actionMinimize->setIcon ( getIcon ( "minimize" ) );
  actionMinimize->setShortcut( Qt::CTRL + Qt::Key_H );

  actionQuit->setIcon ( getIcon ( "application-exit" ) );
  actionQuit->setShortcut( QKeySequence::Quit );

  actionSave->setIcon ( getIcon ( "document-save" ) );
  actionSave->setShortcut( QKeySequence::Save );

  actionLoad->setIcon ( getIcon ( "edit-redo" ) );
  actionLoad->setShortcut( QKeySequence::Undo );

  actionRefresh->setIcon ( getIcon ( "view-refresh" ) );
  actionRefresh->setShortcut( QKeySequence::Refresh );
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
  systemTrayMenu = new QMenu ( this );
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

  systemTrayIcon = new QSystemTrayIcon ( this );
  systemTrayIcon->setIcon ( getIcon ( "qx11grab" ) );
  systemTrayIcon->setToolTip ( trUtf8 ( "qx11grab: recording X11 Windows with ffmpeg" ) );
  systemTrayIcon->setContextMenu ( systemTrayMenu );
  connect ( systemTrayIcon, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ),
            this, SLOT ( systemTrayWatcher ( QSystemTrayIcon::ActivationReason ) ) );
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

void QX11Grab::loadStats()
{
  if ( cfg->contains ( "windowPos" ) )
    move ( cfg->value ( "windowPos", pos() ).toPoint() );

  if ( cfg->contains ( "windowSize" ) )
    resize ( cfg->value ( "windowSize", size() ).toSize() );

  if ( ! cfg->contains ( "Version" ) )
    cfg->setValue ( "Version", QX11GRAB_VERSION );

  loadSettings();
}

void QX11Grab::saveStats()
{
  cfg->setValue ( "windowState", saveState() );
  cfg->setValue ( "windowPos", pos() );
  cfg->setValue ( "windowSize", size() );
}

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

void QX11Grab::showEvent ( QShowEvent * )
{
  loadStats();
}

void QX11Grab::hideEvent ( QHideEvent * )
{
  saveStats();
}

void QX11Grab::closeEvent ( QCloseEvent *ev )
{
  if ( m_FFProcess->isRunning() )
  {
    QMessageBox::warning ( this, trUtf8 ( "Warning" ), trUtf8 ( "Recorder is running." ) );
    ev->ignore();
  }
  saveStats();

#ifdef HAVE_DBUS

  if ( m_QX11GrabAdaptor )
  {
    QDBusConnection::sessionBus().unregisterObject ( "/qx11grab", QDBusConnection::UnregisterNode );
    delete m_QX11GrabAdaptor;
  }

#endif

}

void QX11Grab::pushInfoMessage ( const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->showMessage ( trUtf8 ( "Info" ), txt,
                                  QSystemTrayIcon::Information, TimeOutMessages );

  if ( ! m_FFProcess->isRunning() )
    systemTrayIcon->setIcon ( getIcon ( "qx11grab" ) );
}

void QX11Grab::pushErrorMessage ( const QString &title, const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->showMessage ( title, txt, QSystemTrayIcon::Critical, TimeOutMessages );

  if ( ! m_FFProcess->isRunning() )
    systemTrayIcon->setIcon ( getIcon ( "qx11grab" ) );
}

void QX11Grab::pushToolTip ( const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->setToolTip ( txt );
}

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
      systemTrayIcon->setIcon ( getIcon ( "convert" ) );
    }
  }
  else
    QMessageBox::critical ( this, trUtf8 ( "Error" ), trUtf8 ( "qx11grap not started" ) );
}

void QX11Grab::setActionsBack()
{
  stopRecordingWindow->setEnabled ( false );
  actionStopRecord->setEnabled ( false );
  actionKillRecord->setEnabled ( false );
  startRecordingWindow->setEnabled ( true );
  actionStartRecord->setEnabled ( true );
  systemTrayIcon->setIcon ( getIcon ( "qx11grab" ) );
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

void QX11Grab::perparePreview()
{
  commandLineEdit->clear();

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
  commandLine << m_metaData->getCmd ();
  commandLine << m_defaults->ossdevice();
  commandLine << m_audioEditor->getCmd ();

  // Output Options
  commandLine << "-y" << m_defaults->output();

  commandLineEdit->setPlainText ( commandLine.join ( " " ) );

  cfg->setValue ( QLatin1String ( "CurrentCommandLine" ), commandLine );
}

QX11Grab::~QX11Grab()
{
  if ( m_FFProcess->isRunning() )
    m_FFProcess->kill();
}

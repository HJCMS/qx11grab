/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "qx11grab.h"
#include "settings.h"
#include "desktopinfo.h"
#include "rubberband.h"
#include "windowgrabber.h"
#include "settingsdialog.h"
#include "ffprocess.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>
#include <QtGui/QDesktopWidget>
#include <QtGui/QRubberBand>
#include <QtGui/QMessageBox>

QX11Grab::QX11Grab( Settings *settings )
  : cfg ( settings )
{
  setupUi ( this );

  setDepthBox->hide();
  TimeOutMessages = 5000;

  loadStats();

  createActions();
  createEnviroment();
  createSystemTrayIcon();

  /* now bind ffprocess */
  m_FFProcess = new FFProcess ( this, cfg );

  /* Signals */
  connect ( m_FFProcess, SIGNAL ( message ( const QString & ) ),
            this, SLOT ( pushInfoMessage ( const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( errmessage ( const QString &, const QString & ) ),
            this, SLOT ( pushErrorMessage ( const QString &, const QString & ) ) );

  connect ( m_FFProcess, SIGNAL ( trigger ( const QString & ) ),
            this, SLOT ( pushToolTip ( const QString & ) ) );

  connect ( screenComboBox, SIGNAL ( screenNameChanged ( const QString & ) ),
            setModeName, SLOT ( setText ( const QString & ) ) );

  connect ( screenComboBox, SIGNAL ( screenWidthChanged ( int ) ),
            setWidthBox, SLOT ( setValue ( int ) ) );

  connect ( screenComboBox, SIGNAL ( screenHeightChanged ( int ) ),
            setHeightBox, SLOT ( setValue ( int ) ) );

  connect ( screenComboBox, SIGNAL ( screenDepthChanged ( int ) ),
            setDepthBox, SLOT ( setValue ( int ) ) );

  connect ( setWidthBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( toRubber ( int ) ) );

  connect ( setHeightBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( toRubber ( int ) ) );

  connect ( setXBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( toRubber ( int ) ) );

  connect ( setYBox, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( toRubber ( int ) ) );

  connect ( showRubberband, SIGNAL ( clicked ( bool ) ),
            this, SLOT ( showRubber ( bool ) ) );

  connect ( actionGrabbing, SIGNAL ( triggered () ),
            this, SLOT ( grabFromWindow () ) );

  connect ( actionStartRecord, SIGNAL ( triggered () ),
            this, SLOT ( startRecord () ) );

  connect ( actionStopRecord, SIGNAL ( triggered () ),
            this, SLOT ( stopRecord () ) );

  connect ( actionApplication, SIGNAL ( triggered() ),
            this, SLOT ( openSettings() ) );

  connect ( actionMinimize, SIGNAL ( triggered() ),
            this, SLOT ( hide() ) );

  connect ( actionQuit, SIGNAL ( triggered() ),
            qApp, SLOT ( quit() ) );
}

const QIcon QX11Grab::getIcon ( const QString &name, const QString &group )
{
  QIcon icon;
  QString file = QString::fromUtf8 ( ":%1/images/%2.png" ).arg ( group, name );
  icon.addPixmap ( QPixmap ( file ), QIcon::Normal, QIcon::Off );
  return icon;
}

void QX11Grab::createActions()
{
  grabActionFromWindow = new QAction ( getIcon ( "window" ), trUtf8 ( "Grabbing" ), this );
  connect ( grabActionFromWindow, SIGNAL ( triggered() ), this, SLOT ( grabFromWindow() ) );

  showRubberbandWindow = new QAction ( getIcon ( "grid" ), trUtf8 ( "Rubberband" ), this );
  connect ( showRubberbandWindow, SIGNAL ( triggered() ), this, SLOT ( swapRubberBand() ) );

  startRecordingWindow = new QAction ( getIcon ( "run" ), trUtf8 ( "Record" ), this );
  connect ( startRecordingWindow, SIGNAL ( triggered() ), this, SLOT ( startRecord() ) );

  stopRecordingWindow = new QAction ( getIcon ( "stop" ), trUtf8 ( "Stop" ), this );
  stopRecordingWindow->setEnabled ( false );
  connect ( stopRecordingWindow, SIGNAL ( triggered() ), this, SLOT ( stopRecord() ) );

  minimizeWindowAction = new QAction ( getIcon ( "minimize" ), trUtf8 ( "Hide" ), this );
  connect ( minimizeWindowAction, SIGNAL ( triggered() ), this, SLOT ( hide() ) );

  displayWindowAction = new QAction ( getIcon ( "maximize" ), trUtf8 ( "Show" ), this );
  connect ( displayWindowAction, SIGNAL ( triggered() ), this, SLOT ( showNormal() ) );

  quitWindowAction = new QAction ( getIcon ( "fail" ), trUtf8 ( "Quit" ), this );
  connect ( quitWindowAction, SIGNAL ( triggered() ), qApp, SLOT ( quit() ) );
}

void QX11Grab::createEnviroment()
{
  m_DesktopInfo = new DesktopInfo ( this );
  FrameMode desktop = m_DesktopInfo->grabScreenGeometry ( centralWidget() );
  setDepthBox->setValue ( desktop.depth );

  m_RubberBand = new RubberBand ( qApp->desktop()->screen() );
  connect ( m_RubberBand, SIGNAL ( error ( const QString &, const QString & ) ),
            this, SLOT ( pushErrorMessage ( const QString &, const QString & ) ) );

  toRubber ( 1 );
  if ( showRubberband->isChecked() )
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
  if ( showRubberband->isChecked() )
  {
    showRubberband->setChecked ( false );
    m_RubberBand->hide();
  }
  else
  {
    showRubberband->setChecked ( true );
    m_RubberBand->show();
  }
}

void QX11Grab::showRubber ( bool b )
{
  if ( b )
  {
    showRubberband->setChecked ( true );
    m_RubberBand->show();
  }
  else
  {
    showRubberband->setChecked ( false );
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
  {
    cfg->setValue ( "Version", QX11GRAB_VERSION );
    openSettings();
  }
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

  int w = setWidthBox->value();
  int h = setHeightBox->value();
  int x = setXBox->value();
  int y = setYBox->value();
  m_RubberBand->resize ( w, h );
  m_RubberBand->move ( x, y );
}

void QX11Grab::grabFromWindow()
{
  if ( ! m_RubberBand )
    return;

  WindowGrabber *grabber = new WindowGrabber ( this );
  QRect rect = grabber->grabWindowRect();

  if ( rect.isValid() )
  {
    setWidthBox->setValue ( rect.width() );
    setHeightBox->setValue ( rect.height() );
    setXBox->setValue ( rect.x() );
    setYBox->setValue ( rect.y() );
    setModeName->setText ( trUtf8 ( "grabbed Dimension" ) );
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

void QX11Grab::openSettings ()
{
  SettingsDialog *confDialog = new SettingsDialog ( centralWidget(), cfg );
  confDialog->exec();
  delete confDialog;
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
}

void QX11Grab::pushInfoMessage ( const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->showMessage ( trUtf8 ( "Info" ), txt,
                                  QSystemTrayIcon::Information, TimeOutMessages );
}

void QX11Grab::pushErrorMessage ( const QString &title, const QString &txt )
{
  if ( systemTrayIcon )
    systemTrayIcon->showMessage ( title, txt, QSystemTrayIcon::Critical, TimeOutMessages );
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

  QRect rect ( setXBox->value(), setYBox->value(), setWidthBox->value(), setHeightBox->value() );
  if ( m_FFProcess->create ( rect ) )
  {
    stopRecordingWindow->setEnabled ( true );
    actionStopRecord->setEnabled ( true );
    startRecordingWindow->setEnabled ( false );
    actionStartRecord->setEnabled ( false );
    showRubber ( false );
    if ( m_FFProcess->start() )
    {
      systemTrayIcon->setIcon ( getIcon ( "convert" ) );
    }
  }
  else
    QMessageBox::critical ( this, trUtf8 ( "Error" ), trUtf8 ( "qx11grap not started" ) );
}

void QX11Grab::stopRecord()
{
  if ( m_FFProcess->stop() )
  {
    stopRecordingWindow->setEnabled ( false );
    actionStopRecord->setEnabled ( false );
    startRecordingWindow->setEnabled ( true );
    actionStartRecord->setEnabled ( true );
    systemTrayIcon->setIcon ( getIcon ( "qx11grab" ) );
  }
}

QX11Grab::~QX11Grab()
{
}

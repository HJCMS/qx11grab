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

QX11Grab::QX11Grab()
{
  setupUi ( this );

  cfg = new Settings ( this );
  cfg->setValue ( "Version", QX11GRAB_VERSION );

  loadStats();

  createActions();
  createEnviroment();
  createSystemTrayIcon();

  /* Buttons */
  cursorGrabButton = new QPushButton ( trUtf8 ( "Window" ) );
  cursorGrabButton->setToolTip ( trUtf8 ( "grab from Window" ) );
  cursorGrabButton->setStatusTip ( trUtf8 ( "grab from Window" ) );
  buttonBox->addButton ( cursorGrabButton, QDialogButtonBox::ActionRole );

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

  connect ( cursorGrabButton, SIGNAL ( clicked () ),
            this, SLOT ( grabFromWindow () ) );

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
  minimizeWindowAction = new QAction ( getIcon ( "grid" ), trUtf8 ( "Hide" ), this );
  connect ( minimizeWindowAction, SIGNAL ( triggered() ), this, SLOT ( hide() ) );

  displayWindowAction = new QAction ( getIcon ( "fail" ), trUtf8 ( "Show" ), this );
  connect ( displayWindowAction, SIGNAL ( triggered() ), this, SLOT ( showNormal() ) );

  quitWindowAction = new QAction ( getIcon ( "stop" ), trUtf8 ( "Quit" ), this );
  connect ( quitWindowAction, SIGNAL ( triggered() ), qApp, SLOT ( quit() ) );
}

void QX11Grab::createEnviroment()
{
  m_DesktopInfo = new DesktopInfo ( this );
  FrameMode desktop = m_DesktopInfo->grabScreenGeometry ( centralWidget() );
  setDepthBox->setValue ( desktop.depth );

  m_RubberBand = new RubberBand ( qApp->desktop()->screen() );
  if ( showRubberband->isChecked() )
    m_RubberBand->show();
  else
    m_RubberBand->hide();
}

void QX11Grab::createSystemTrayIcon()
{
  systemTrayMenu = new QMenu ( this );
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

void QX11Grab::showRubber ( bool b )
{
  if ( b )
    m_RubberBand->show();
  else
    m_RubberBand->hide();
}

void QX11Grab::loadStats()
{
  if ( cfg->contains ( "windowPos" ) )
    move ( cfg->value ( "windowPos", pos() ).toPoint() );

  if ( cfg->contains ( "windowSize" ) )
    resize ( cfg->value ( "windowSize", size() ).toSize() );

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
    setWidthBox->setValue( rect.width() );
    setHeightBox->setValue( rect.height() );
    setXBox->setValue( rect.x() );
    setYBox->setValue( rect.y() );
    setModeName->setText ( trUtf8( "grabbed Dimension" ) );
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

void QX11Grab::closeEvent ( QCloseEvent * )
{
  saveStats();
}

QX11Grab::~QX11Grab()
{}

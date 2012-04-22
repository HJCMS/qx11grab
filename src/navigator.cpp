/*
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

#include "navigator.h"

/* QX11Grab */
#include "settings.h"
#include "playeraction.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QMargins>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QCursor>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLinearGradient>
#include <QtGui/QMenu>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionToolButton>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusConnection>

/** \class NavAction */
NavAction::NavAction ( const QString &toolTip, const QString &iconName, QWidget * parent )
    : QToolButton ( parent )
{
  setToolButtonStyle ( Qt::ToolButtonIconOnly );
  setStatusTip ( toolTip );
  setIcon ( Settings::themeIcon ( iconName ) );
}

void NavAction::paintEvent ( QPaintEvent * event )
{
  QStyleOptionToolButton option;
  initStyleOption ( &option );

  QToolButton::paintEvent ( event );
}

/** \class Navigator */
Navigator::Navigator ( QDesktopWidget * parent )
    : QWidget ( parent )
    , m_desktop ( parent )
    , m_state ( 0 )
    , m_settings ( new Settings ( parent ) )
{
  setObjectName ( QLatin1String ( "Navigation" ) );
  setWindowFlags ( ( Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint ) );
  setWindowModality ( Qt::NonModal );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_Hover, true );
  setAttribute ( Qt::WA_OpaquePaintEvent, true );
  setAttribute ( Qt::WA_PaintOnScreen, true );
  // wird hier nicht benötigt
  setMouseTracking ( false );
  setAcceptDrops ( false );
  // x11 composite hack
  setWindowOpacity ( 1.0 );
  // genügent anfasser breich zu verfügung stellen
  QMargins mrg = contentsMargins();
  mrg.setRight ( 8 );
  setContentsMargins ( mrg );
  // ausblenden anbieten
  setContextMenuPolicy ( Qt::DefaultContextMenu );

  setAccessibleName ( QLatin1String ( "Toolbar" ) );
  setAccessibleDescription ( QLatin1String ( "Navigation Toolbar" ) );

  QHBoxLayout* layout = new QHBoxLayout ( this );
  layout->setSpacing ( 6 );

  m_actionStartRecord = new NavAction ( trUtf8 ( "Start Recording" ), QString ( "media-record" ), this );
  layout->addWidget ( m_actionStartRecord );

  m_actionStopRecord = new NavAction ( trUtf8 ( "Stop Recording" ), QString ( "media-playback-stop" ), this );
  m_actionStopRecord->setEnabled ( false );
  layout->addWidget ( m_actionStopRecord );

  m_rubberbandAction = new NavAction ( trUtf8 ( "swap rubberband view" ), QString ( "view-grid" ), this );
  layout->addWidget ( m_rubberbandAction );

  m_playerAction = new PlayerAction ( this );
  m_playerAction->setEnabled ( false );
  layout->addWidget ( m_playerAction );

  m_infoData = new QLineEdit ( this );
  m_infoData->setReadOnly ( true );
  m_infoData->setEnabled ( false );
  m_infoData->setMinimumWidth ( 230 );
  layout->addWidget ( m_infoData );

  layout->addStretch();

  setLayout ( layout );

  QDBusConnection p_dbus = QDBusConnection::sessionBus();
  p_dbus.registerObject ( "/Navigation", this, ( QDBusConnection::ExportScriptableSlots ) );

  // Default Actions
  connect ( m_actionStartRecord, SIGNAL ( clicked() ), this, SIGNAL ( startRecord() ) );
  connect ( m_actionStopRecord, SIGNAL ( clicked() ), this, SIGNAL ( stopRecord() ) );
  connect ( m_rubberbandAction, SIGNAL ( clicked() ), this, SIGNAL ( rubberBand() ) );
}

/** Initialisiere die Status Informationen */
void Navigator::initMoveState ( const QPoint &p )
{
  if ( m_state != 0 )
    return;

  m_state = new MoveState;
  m_state->position = p;
  m_state->moving = false;
}

/** Ändere beim Verschieben die Status Informationen */
void Navigator::startMoveWidget ( bool b )
{
  Q_ASSERT ( m_state != 0 );
  if ( b && m_state->moving )
    return;

  m_state->moving = b;
}

/**
* Beende das Verschieben und räume auf
* \warning Kein Assert verwenden siehe leaveEvent!
*/
void Navigator::stopMoveWidget()
{
  if ( m_state == 0 )
    return;

  releaseMouse();

  delete m_state;
  m_state = 0;
  repaint();
}

/** Ausblenden Menü anzeigen */
void Navigator::contextMenuEvent ( QContextMenuEvent * event )
{
  QMenu* m = new QMenu ( this );

  QAction* m_hideAction = m->addAction ( Settings::themeIcon ( "dialog-close" ), trUtf8 ( "Close" ) );
  connect ( m_hideAction, SIGNAL ( triggered() ), this, SLOT ( hide() ) );

  m->addSeparator();

  QAction* ac_hideWindow = m->addAction ( Settings::themeIcon ( "minimize" ), trUtf8 ( "Hide Window" ) );
  connect ( ac_hideWindow, SIGNAL ( triggered() ), this, SIGNAL ( hideMainWindow() ) );
  m->addAction ( ac_hideWindow );

  QAction* ac_showWindow = m->addAction ( Settings::themeIcon ( "maximize" ), trUtf8 ( "Show Window" ) );
  connect ( ac_showWindow, SIGNAL ( triggered() ), this, SIGNAL ( showMainWindow() ) );
  m->addAction ( ac_showWindow );

  m->exec ( event->globalPos() );
  delete m;
  repaint();
}

/**
* Wir zeichnen ein Widget ohne Fenster Dekoration mit runden Ecken.
* \note siehe im Konstruktor setWindowFlags und setAttribute
*/
void Navigator::paintEvent ( QPaintEvent * event )
{
  QPainter painter ( this );
  painter.setRenderHint ( QPainter::Antialiasing );
#ifdef HAVE_OPENGL
  painter.setCompositionMode ( QPainter::CompositionMode_Overlay );
#else
  painter.setCompositionMode ( QPainter::CompositionMode_SourceOver );
#endif
  painter.setBackgroundMode ( Qt::TransparentMode );
  painter.setBackground ( Qt::NoBrush );
  painter.setPen ( Qt::NoPen );

  QPalette pal = qApp->palette();
  QRect r = event->rect();
  qreal radius = ( r.height() / 3 );

  QLinearGradient gradient ( 0.0, 0.0, 0.0, r.height() );
  gradient.setColorAt ( 0.0, pal.dark().color() );
  gradient.setColorAt ( 0.5, pal.light().color() );
  gradient.setColorAt ( 1.0, pal.dark().color() );
  painter.setBrush ( gradient );

  QPainterPath rectPath;
  rectPath.setFillRule ( Qt::WindingFill );
  rectPath.addRoundedRect ( 0.0, 0.0, r.width(), r.height(), radius, radius, Qt::AbsoluteSize );
  painter.drawPath ( rectPath );
}

/** Wenn das Widget ausgeblendet wird die Position speichern. */
void Navigator::hideEvent ( QHideEvent * event )
{
  m_settings->setValue ( "Navigator/Rect", frameGeometry() );
  m_infoData->clear();
  event->ignore();
}

/**
* Beim einblenden die Position aus der Konfiguration laden
* \note Wir verändern \b NICHT die Breite und Höhe!
*/
void Navigator::showEvent ( QShowEvent * event )
{
  QRect r = m_settings->value ( "Navigator/Rect", QRect ( 10, 10, 100, 24 ) ).toRect();
  r.setWidth ( rect().width() );
  r.setHeight ( rect().height() );
  // FIXME Wenn der Benutzer verschieden Layouts verwendet darauf Achten das
  // nach einem wechsle des Xserver-Layouts dieses Widget in einem Sichtbaren Bereich liegt!
  if ( m_desktop->availableGeometry ( m_desktop->primaryScreen() ).contains ( r.topLeft() ) )
  {
    setGeometry ( r );
    event->ignore();
  }
  else
  {
    qWarning ( "Navigator isn't visible!" );
    QWidget::showEvent ( event );
  }
}

/**
* Widget verschieben verarbeiten
*/
void Navigator::mouseMoveEvent ( QMouseEvent * event )
{
  if ( ! m_state || ! isVisible() )
  {
    event->ignore();
    return;
  }

  // qDebug() << "MouseMoveEvent Positions - WIDGET:" << event->pos() << " GLOBAL:" << event->globalPos();;

  if ( ( event->pos() - m_state->position ).manhattanLength() > qApp->startDragDistance() )
  {
    const bool m = ( ( ( event->x() >= 0 ) && ( event->x() < m_desktop->width() ) )
                     || ( ( event->y() >= 0 ) && ( event->y() < m_desktop->height() ) ) );

    startMoveWidget ( m );

    if ( ! m )
    {
      qWarning ( "Using buggy grab the mouse input." );
      grabMouse();
    }
  }

  QPoint pos = event->globalPos();
  // passend zur Layoutrichtung
  if ( m_desktop->isLeftToRight() )
    pos -= m_state->position;
  else
    pos += QPoint ( ( m_state->position.x() - m_desktop->width() ), -m_state->position.y() );

  move ( pos );
}

/**
* Wenn die Linke Maustaste gedrückt wird
* Die Verschieben aktionen Initialisieren
*/
void Navigator::mousePressEvent ( QMouseEvent * event )
{
  if ( event->button() == Qt::LeftButton )
  {
    setCursor ( Qt::ClosedHandCursor );
    initMoveState ( event->pos() );
  }
  else
    event->ignore();
}

/**
* Wenn die Maus los gelassen wird alles zurück setzen
*/
void Navigator::mouseReleaseEvent ( QMouseEvent * event )
{
  if ( event->button() == Qt::LeftButton )
    stopMoveWidget ();
  else
    event->ignore();

  setCursor ( Qt::ArrowCursor );
}

/**
* Infoanzeige erneuern
*/
void Navigator::setInfo ( const QString &info )
{
  m_infoData->setText ( info );
}

/**
* Status der Knöpfe ändern
*/
void Navigator::setActivity ( bool b )
{
  bool swap = ( ( b ) ? false : true );
  m_actionStopRecord->setEnabled ( b );
  m_actionStartRecord->setEnabled ( swap );
  if ( swap )
  {
    QFileInfo file ( m_settings->absoluteOutputPath() );
    setPlayerEnabled ( file.exists() );
  }
}

/**
* Videomenü Aktivieren
*/
void Navigator::setPlayerEnabled ( bool b )
{
  m_playerAction->setEnabled ( b );
}

Navigator::~Navigator()
{
  if ( m_state )
  {
    delete m_state;
    m_state = 0;
  }

  if ( m_settings )
    delete m_settings;
}

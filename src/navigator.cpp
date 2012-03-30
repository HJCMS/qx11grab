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

#include "navigator.h"

/* QX11Grab */
#include "settings.h"
#include "menubar.h"
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
#include <QtGui/QLinearGradient>
#include <QtGui/QMenu>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QVBoxLayout>

/* QtDBus */
#include <QtDBus/QDBusConnection>

Navigator::Navigator ( QWidget * parent )
    : QWidget ( parent )
    , m_state ( 0 )
    , m_settings ( new Settings ( parent ) )
{
  setObjectName ( QLatin1String ( "Navigation" ) );
  setWindowFlags ( ( Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint ) );
  setWindowModality ( Qt::NonModal );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_Hover, true );
  setAttribute ( Qt::WA_X11NetWmWindowTypeToolBar, true );
  setAttribute ( Qt::WA_OpaquePaintEvent, true );
  setAttribute ( Qt::WA_PaintOnScreen, true );
  // wird hier nicht benötigt
  setMouseTracking ( false );
  // x11 composite hack
  setWindowOpacity ( 1.0 );
  // genügent anfasser breich zu verfügung stellen
  QMargins mrg = contentsMargins();
  mrg.setRight ( 8 );
  setContentsMargins ( mrg );
  // ausblenden anbieten
  setContextMenuPolicy ( Qt::DefaultContextMenu );

  QHBoxLayout* layout = new QHBoxLayout ( this );
  layout->setSpacing ( 6 );

  m_actionStartRecord = new QToolButton ( this );
  m_actionStartRecord->setToolButtonStyle ( Qt::ToolButtonIconOnly );
  /*: ToolTip */
  m_actionStartRecord->setStatusTip ( trUtf8 ( "Start Recording" ) );
  m_actionStartRecord->setIcon ( Settings::themeIcon ( "media-record" ) );
  layout->addWidget ( m_actionStartRecord );

  m_actionStopRecord = new QToolButton ( this );
  m_actionStopRecord->setToolButtonStyle ( Qt::ToolButtonIconOnly );
  /*: ToolTip */
  m_actionStopRecord->setStatusTip ( trUtf8 ( "Stop Recording" ) );
  m_actionStopRecord->setIcon ( Settings::themeIcon ( "media-playback-stop" ) );
  m_actionStopRecord->setEnabled ( false );
  layout->addWidget ( m_actionStopRecord );

  m_rubberbandAction = new QToolButton ( this );
  m_rubberbandAction->setToolButtonStyle ( Qt::ToolButtonIconOnly );
  /*: ToolTip */
  m_rubberbandAction->setStatusTip ( trUtf8 ( "swap rubberband view" ) );
  m_rubberbandAction->setIcon ( Settings::themeIcon ( "view-grid" ) );
  layout->addWidget ( m_rubberbandAction );

  m_playerAction = new PlayerAction ( this );
  m_playerAction->setEnabled ( false );
  layout->addWidget ( m_playerAction );

  m_infoData = new QLineEdit ( this );
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

/**
* Initialisiere die Status Informationen
*/
void Navigator::initMoveState ( const QPoint &p )
{
  if ( m_state != 0 )
    return;

  m_state = new MoveState;
  m_state->widget = qApp->desktop();
  m_state->startPos = p;
  m_state->move = false;
}

/**
* Ändere beim Verschieben die Status Informationen
*/
void Navigator::startMoveWidget ( bool b )
{
  Q_ASSERT ( m_state != 0 );
  if ( ( b && m_state->move ) )
    return;

  m_state->move = b;
  setCursor ( Qt::ClosedHandCursor );
}

/**
* Beende das Verschieben und räume auf
*/
void Navigator::stopMoveWidget()
{
  Q_ASSERT ( m_state != 0 );
  m_state->widget = 0;
  delete m_state;
  m_state = 0;
  setCursor ( Qt::ArrowCursor );
}

/**
* Ausblenden Menü anzeigen
*/
void Navigator::contextMenuEvent ( QContextMenuEvent * event )
{
  QMenu* m = new QMenu ( this );
  QAction* m_hideAction = MenuBar::hideWindowAction ( m );
  connect ( m_hideAction, SIGNAL ( triggered() ), this, SLOT ( hide() ) );
  m->addAction ( m_hideAction );
  m->exec ( event->globalPos() );
  delete m;
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
  rectPath.addRoundedRect ( 0.0, 0.0, r.width(), r.height(), radius, radius, Qt::AbsoluteSize );
  rectPath.setFillRule ( Qt::WindingFill );
  painter.drawPath ( rectPath );
}

/**
* Wenn das Widget ausgeblendet wird die Position speichern.
*/
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
  setGeometry ( r );
  event->ignore();
}

/**
* Widget verschieben verarbeiten
*/
void Navigator::mouseMoveEvent ( QMouseEvent * event )
{
  if ( ! m_state || ! m_state->widget )
  {
    event->ignore();
    return;
  }

  if ( ( event->pos() - m_state->startPos ).manhattanLength() > qApp->startDragDistance() )
  {
    const bool move = ( ( ! m_state->widget->isWindow() ) ?
                        ( ( event->x() >= 0 ) && ( event->x() < m_state->widget->width() ) ) :
                        ( ( event->y() >= 0 ) && ( event->y() < m_state->widget->height() ) ) );

    startMoveWidget ( move );

    if ( ! move )
      m_state->widget->grabMouse();
  }

  if ( m_state->move )
  {
    QPoint pos = event->globalPos();
    // passend zur Layoutrichtung
    if ( qApp->isLeftToRight() )
      pos -= m_state->startPos;
    else
      pos += QPoint ( ( m_state->startPos.x() - m_state->widget->width() ), -m_state->startPos.y() );

    move ( pos );
  }
}

/**
* Wenn die Maus die Linke Maustaste gedrückt wird
* Die Verschieben aktionen Initialisieren
*/
void Navigator::mousePressEvent ( QMouseEvent * event )
{
  if ( event->button() == Qt::LeftButton )
    initMoveState ( event->pos() );
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

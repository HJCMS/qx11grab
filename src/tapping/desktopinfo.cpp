/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#include "desktopinfo.h"
#include "desktoptapping.h"

#include <cstdlib>

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QSize>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QX11Info>

/**
* @class DesktopInfo
* Erstellt Desktop Informationen vom aktuellen Bildschirm
*/
DesktopInfo::DesktopInfo ( QObject * parent )
    : QObject ( parent )
    , m_desktopWidget ( qApp->desktop() )
{
  connect ( m_desktopWidget, SIGNAL ( resized ( int ) ),
            this, SIGNAL ( resized ( int ) ) );

  connect ( m_desktopWidget, SIGNAL ( workAreaResized ( int ) ),
            this, SIGNAL ( resized ( int ) ) );
}

/**
* Berechnet das Seitenverhältnis
*/
qreal DesktopInfo::todar ( int w, int h ) const
{
  char* dar = new char[7];
  qreal width = static_cast<qreal> ( w );
  qreal height = static_cast<qreal> ( h );
  sprintf ( dar, "%0.4f", ( width / height ) );
  return std::atof ( dar );
}

/**
* Generiert einen FrameMode Eintrag.
*/
const DesktopInfo::FrameMode DesktopInfo::generateFrameMode ( const QString &n, int w, int h, qreal r )
{
  FrameMode mode;
  QString info ( n );
  info.append ( " (" );
  info.append ( QString::number ( w ) );
  info.append ( "x" );
  info.append ( QString::number ( h ) );
  info.append ( ")" );

  mode.name = n;
  mode.width = w;
  mode.height = h;
  mode.depth = getDepth();
  mode.summary = info;
  mode.dar = ( ( r == 0.0 ) ? todar ( w, h ) : r );

  return mode;
}

/**
* Liste der Standardtisierten Modi in FFmpeg
*/
const QList<DesktopInfo::FrameMode> DesktopInfo::modes ( QWidget * parent )
{
  QList<FrameMode> buf;
  buf.append ( generateFrameMode ( "sqcif", 128, 96 ) );
  buf.append ( generateFrameMode ( "qcif", 176, 144 ) );
  buf.append ( generateFrameMode ( "cga", 320, 200 ) );
  buf.append ( generateFrameMode ( "240p (LDTV)", 320, 240 ) );
  buf.append ( generateFrameMode ( "cif (VCD)", 352, 288 ) );
  buf.append ( generateFrameMode ( "iPhone", 480, 320 ) );
  buf.append ( generateFrameMode ( "ega", 640, 350 ) );
  buf.append ( generateFrameMode ( "360p (LDTV)", 640, 360 ) );
  buf.append ( generateFrameMode ( "4cif", 704, 576 ) );
  buf.append ( generateFrameMode ( "qqvga", 160, 120 ) );
  buf.append ( generateFrameMode ( "qvga", 320, 240 ) );
  buf.append ( generateFrameMode ( "vga", 640, 480 ) );
  buf.append ( generateFrameMode ( "dvd (PAL)", 720, 576 ) );
  buf.append ( generateFrameMode ( "svga", 800, 600 ) );
  buf.append ( generateFrameMode ( "hd480", 852, 480 ) );
  buf.append ( generateFrameMode ( "xga", 1024, 768 ) );
  buf.append ( generateFrameMode ( "hd720 (EDTV)", 1280, 720 ) );
  buf.append ( generateFrameMode ( "uxga", 1600, 1200 ) );
  buf.append ( generateFrameMode ( "hd1080 (HDTV)", 1920, 1080 ) );
  buf.append ( generateFrameMode ( "qxga", 2048, 1536 ) );
  buf.append ( generateFrameMode ( "sxga", 1280, 1024 ) );
  buf.append ( generateFrameMode ( "sxga+", 1440, 1050 ) );
  buf.append ( generateFrameMode ( "qsxga", 2560, 2048 ) );
  buf.append ( generateFrameMode ( "hsxga", 5120, 4096 ) );
  buf.append ( generateFrameMode ( "wvga", 852, 480 ) );
  buf.append ( generateFrameMode ( "wxga", 1360, 768 ) );
  buf.append ( generateFrameMode ( "wxga+", 1440, 900 ) );
  buf.append ( generateFrameMode ( "wsxga", 1600, 1024 ) );
  buf.append ( generateFrameMode ( "wsxga+", 1680, 1050 ) );
  buf.append ( generateFrameMode ( "wuxga", 1920, 1200 ) );
  buf.append ( generateFrameMode ( "woxga", 2560, 1600 ) );
  buf.append ( generateFrameMode ( "wqsxga", 3200, 2048 ) );
  buf.append ( generateFrameMode ( "wquxga", 3840, 2400 ) );
  buf.append ( generateFrameMode ( "whsxga", 6400, 4096 ) );
  buf.append ( generateFrameMode ( "whuxga", 7680, 4800 ) );

  if ( parent )
    buf.append ( grabScreenGeometry ( parent ) );

  QList<FrameMode> out;
  int mw = getMaxWidth();
  int mh = getMaxHeight();
  for ( int i = 0; i < buf.size(); ++i )
  {
    FrameMode m = buf.at ( i );
    if ( ( m.width > mw ) || ( m.height > mh ) )
      continue;

    out.append ( m );
  }
  buf.clear();

  return out;
}

/** Aktuell verwendeter Bildschirm */
int DesktopInfo::getScreen()
{
  return m_desktopWidget->primaryScreen();
}

/**
* Maximale breite des aktuellen Bildschirms
*/
int DesktopInfo::getMaxWidth()
{
  return screenGeometry ( getScreen() ).width();
}

/**
* Maximale höhe des aktuellen Bildschirms
*/
int DesktopInfo::getMaxHeight()
{
  return screenGeometry ( getScreen() ).height();
}

/**
* Aktuellen Farbtiefe des Bildschirms
*/
int DesktopInfo::getDepth()
{
  return QX11Info::appDepth ( getScreen() );
}

/**
* Globales Bildschirm Widget
*/
QWidget* DesktopInfo::screenWidget()
{
  return m_desktopWidget->screen ( getScreen() );
}

/**
* Aktuelle Bildschirm Geometrie
*/
const QRect DesktopInfo::screenGeometry ( int screen )
{
  return m_desktopWidget->screenGeometry ( screen );
}

/**
* Sucht in der Modi Liste nach einem Eintrag
*/
const DesktopInfo::FrameMode DesktopInfo::getFrameMode ( const QString &name, QWidget *parent )
{
  if ( parent )
  {
    foreach ( FrameMode mode, modes ( parent ) )
    {
      if ( mode.name == name )
        return mode;
    }
  }
  return generateFrameMode ( trUtf8 ( "Unknown" ), 640, 480 );
}

/**
* Abgreifen der Geometrie eines Desktop Widgets
*/
const DesktopInfo::FrameMode DesktopInfo::grabScreenGeometry ( QWidget * parent )
{
  int sc = ( parent ) ? m_desktopWidget->screenNumber ( parent ) : getScreen();
  QRect rect = screenGeometry ( sc );
  if ( ! rect.isValid() )
    return generateFrameMode ( trUtf8 ( "Unknown" ), 640, 480 );

  QSize size = rect.size();
  return generateFrameMode ( trUtf8 ( "Fullscreen" ), size.width(), size.height() );
}

DesktopInfo::~DesktopInfo()
{
  if ( m_desktopWidget )
    delete m_desktopWidget;
}

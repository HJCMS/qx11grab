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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#include "desktoptapping.h"

#include <ctype.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QPoint>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QX11Info>

/* X11 */
// FIXME qwindowdefs.h
#ifdef Q_WS_X11
# define XLIB_ILLEGAL_ACCESS 1
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#define MASK (ButtonPressMask | ButtonReleaseMask)

DesktopTapping::DesktopTapping ( QObject * parent )
    : QDesktopWidget ()
{
  Q_CHECK_PTR ( parent );
  setParent ( qApp->desktop() );
  setMouseTracking ( false );
}

/**
* Weil Qt bei TwinView, Cloned Desktops und Mehrfachen XServer
* Bildschirm Layouts nicht den Richtigen Screen erkennen kann.
* An dieser Stelle gegen die Globalen Variable \b $DISPLAY Validieren.
* Grundsätzlich gilt - \b $DISPLAY hat vorrang!
* 
* Wenn der Benutzer mehrere Bildschirm Layouts verwendet kann die Anzahl der Bildschirme Nummern
* stark variieren. Beispiel Konfiguration mit mehreren Layouts aus /etc/X11/xdm/Xservers entnommen.
* \code
*   :0 local /usr/bin/X -nolisten tcp -config xorg.conf -layout XineramaLayout +xinerama
*   :1 local /usr/bin/X -nolisten tcp -config xorg.conf -layout SingleLayout -xinerama
*   u.s.w.
* \endcode
*
* oder $KDEDIRS/config/kdm/kdmrc
* \code
*   [X-:0-Core]
*   ServerArgsLocal=-nolisten tcp -layout XineramaLayout +xinerama -extension RandR -extension Composite
* 
*   [X-:1-Core]
*   ServerArgsLocal=-nolisten tcp -layout SingleLayout -xinerama +extension RandR +extension Composite
*
*   u.s.w.
* \endcode
*/
int DesktopTapping::realDesktopScreen ( int screen ) const
{
  int ret = screen;
  QByteArray x = qgetenv ( "DISPLAY" );
  if ( ! x.isNull() && x.contains ( ':' ) )
  {
    // NOTE Es werden im Moment nur Bildschirme von 0-9 gelesen!
    char d = x.at ( ( x.size() - 1 ) );
    int s = atoi ( &d );
    ret = ( s >= 0 ) ? s : screen;
    x.clear();
  }
  return ret;
}

/**
* Werte müssen durch 2 Teilbar sein
* damit FFmpeg sie verwenden kann.
*/
int DesktopTapping::normalize ( int z ) const
{
  return ( ( z % 2 ) != 0 ) ? ( z + 1 ) : z;
}

/**
* Fenster Geometrie auf Bildschirm abgreifen
*/
void DesktopTapping::grabWindowRect ( int screen )
{
  Display* dpy;
  if ( ( dpy = XOpenDisplay ( NULL ) ) == NULL )
  {
    qWarning ( "QX11Grab - Failed to call XOpenDisplay()" );
    return;
  }

  /* programm Hauptfenster */
  Window root = XRootWindow ( dpy, primaryScreen() );

  /* Fenster das ausgewählt wird */
  Window retwin = 0;

  /* setzt den gedrückt status für die Maustaste */
  int pressed = 0;

  /* Fenster Prädikate */
  XWindowAttributes attr;

  /* Einen anderen Cursor setzen */
  Cursor cursor = XCreateFontCursor ( dpy, XC_crosshair );

  if ( cursor == None )
  {
    qWarning ( "Unable to create crosshair cursor." );
    return;
  }

  /* warte auf xterm */
  XSync ( dpy, 0 );

  if ( XGrabPointer ( dpy, root, False, MASK, GrabModeSync, GrabModeAsync,
                      None, cursor, CurrentTime ) != GrabSuccess )
  {
    qWarning ( "Unable to grab cursor." );
    return;
  }

  /* initial Loop */
  while ( retwin == None || pressed != 0 )
  {
    XEvent event;
    XAllowEvents ( dpy, SyncPointer, CurrentTime );
    XWindowEvent ( dpy, root, MASK, &event );

    switch ( event.type )
    {
      case ButtonPress:
        if ( retwin == None )
        {
          retwin = ( ( event.xbutton.subwindow != None ) ? event.xbutton.subwindow : root );
        }
        pressed++;
        continue;

      case ButtonRelease:
        if ( pressed > 0 ) pressed--;
        continue;

      default:
        continue;

    } /* end switch */
  } /* end loop */

  XUngrabPointer ( dpy, CurrentTime );
  XFreeCursor ( dpy, cursor );
  XSync ( dpy, 0 );

  if ( ! retwin )
    return;

  XGetWindowAttributes ( dpy, retwin, &attr );
  QRect rect = QRect ( attr.x, attr.y, normalize ( attr.width ), normalize ( attr.height ) );
  qDebug ( "qx11grab desktop tapping: \"-i %s+%d,%d -s %dx%d\"",
           dpy->display_name, rect.x(), rect.y(), rect.width(), rect.height() );

  if ( rect.isValid() )
    emit rectChanged ( rect, screen );

  XFlush ( dpy );
}

/**
* Startet das abgreifen der Desktop Auflösung
*/
void DesktopTapping::createRequest ( int screen )
{
  grabWindowRect ( realDesktopScreen ( screen ) );
}

/**
* Setze die Maximale Desktop Größe
* von oben Links nach untent Rechts.
* Xinerama mit in begriffen!
*/
const QRect DesktopTapping::fullDesktopsRect ()
{
  QRect rect = screenGeometry ( 0 );
  if ( isVirtualDesktop() )
  {
    QPoint bottomRight;
    int mw = 0, mh = 0;
    for ( int i = 0; i < screenCount(); i++ )
    {
      QRect size = screenGeometry ( i );
      mw = ( size.width() + mw );
      mh = ( size.height() > mh ) ? size.height() : mh;
    }
    // qDebug() << Q_FUNC_INFO << mw << "x" << mh;
    bottomRight.setX ( mw );
    bottomRight.setY ( mh );
    rect = QRect ( QPoint ( 0, 0 ), bottomRight );
  }
  return rect;
}

DesktopTapping::~DesktopTapping()
{}

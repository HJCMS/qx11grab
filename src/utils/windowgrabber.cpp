/**
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
**/

#include "windowgrabber.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

#include <ctype.h>

/* QtCore */
#include <QtCore/QDebug>

// FIXME qwindowdefs.h
#define XLIB_ILLEGAL_ACCESS 1

/* X11 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xrandr.h>

#define MASK (ButtonPressMask | ButtonReleaseMask)

WindowGrabber::WindowGrabber ( QObject *parent )
    : QObject ( parent )
    , QX11Info()
{}

const QRect WindowGrabber::grabWindowRect()
{
  QRect rectVal;
  Display* dpy =  display();
  int curScreen = screen();
  /* the current root */
  Window root = RootWindow ( dpy, curScreen );
  /* the window that got selected */
  Window retwin = 0;
  /* button used to select window */
  int retbutton = -1;
  /* count of number of buttons pressed */
  int pressed = 0;
  /* window Attributes */
  XWindowAttributes attr;
  /* modify cursor */
  Cursor cursor = XCreateFontCursor ( dpy, XC_crosshair );

  if ( cursor == None )
  {
    qWarning ( "Unable to create crosshair cursor." );
    return rectVal;
  }
  /* give xterm a chance */
  XSync ( dpy, 0 );

  if ( XGrabPointer ( dpy, root, False, MASK, GrabModeSync, GrabModeAsync,
                      None, cursor, CurrentTime ) != GrabSuccess )
  {
    qWarning ( "Unable to grab cursor." );
    return rectVal;
  }

  /* initial Loop */
  while ( retwin == None || pressed != 0 )
  {
    XEvent event;
    XAllowEvents ( dpy, SyncPointer, CurrentTime );
    XWindowEvent ( dpy, root, MASK, &event );
    XRRSelectInput ( dpy, root, MASK );

    switch ( event.type )
    {
      case ButtonPress:
        if ( retwin == None )
        {
          retbutton = event.xbutton.button;
          retwin = ( ( event.xbutton.subwindow != None ) ? event.xbutton.subwindow : root );
        }
        pressed++;
        continue;

      case ButtonRelease:
        if ( pressed > 0 ) pressed--;
        continue;

    } /* end switch */
  } /* end loop */

  XUngrabPointer ( dpy, CurrentTime );
  XFreeCursor ( dpy, cursor );
  XSync ( dpy, 0 );

  if ( retwin )
  {
    int nsizes;
    XGetWindowAttributes ( dpy, retwin, &attr );
    rectVal = QRect ( attr.x, attr.y, normalize ( attr.width ), normalize ( attr.height ) );
    // TODO Bildschirme die mit XRandr 1.3 erstellt werden erkennen und ausgeben!
    XRRScreenConfiguration* randrcfg = XRRGetScreenInfo ( dpy, root );
    XRRScreenSize* size = XRRConfigSizes ( randrcfg, &nsizes );
    qDebug ( "grabbing from Desktop(%dx%d@%d): \"-i %s.%d+%d,%d -s %dx%d\"",
             size->width,
             size->height,
             XRRConfigCurrentRate ( randrcfg ),
             dpy->display_name,
             XRRRootToScreen ( dpy, root ),
             attr.x,
             attr.y,
             attr.width,
             attr.height
           );
    XRRFreeScreenConfigInfo ( randrcfg );
  }

  return rectVal;
}

int WindowGrabber::normalize ( int z ) const
{
  return ( ( z % 2 ) != 0 ) ? ( z + 1 ) : z;
}

WindowGrabber::~WindowGrabber()
{}

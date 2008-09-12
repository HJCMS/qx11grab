/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "desktopinfo.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QSize>
#include <QtCore/QRect>

/* QtGui */
#include <QtGui/QApplication>

DesktopInfo::DesktopInfo ( QObject *parent )
    : QObject ( parent )
    , xInfo()
{
  desktopWidget = qApp->desktop();
  Screens = desktopWidget->numScreens();
  Depth = xInfo.depth();
  maxWidth = 640;
  maxHeight = 480;
}

const FrameMode DesktopInfo::fetchFrameMode ( const QString &n, int w, int h )
{
  FrameMode mode;
  QString sum ( n );
  sum.append ( " (" );
  sum.append ( QString::number ( w ) );
  sum.append ( "x" );
  sum.append ( QString::number ( h ) );
  sum.append ( ")" );

  mode.name = n;
  mode.width = w;
  mode.height = h;
  mode.depth = Depth;
  mode.summary = sum;

  return mode;
}

ModeList DesktopInfo::modes ( QWidget *parent )
{
  if ( parent )
  {
    Depth = xInfo.appDepth ( desktopWidget->screenNumber ( parent ) );
    grabScreenGeometry ( parent );
  }

  QList<FrameMode> modes;
  modes.append ( fetchFrameMode ( "sqcif", 128, 96 ) );
  modes.append ( fetchFrameMode ( "qcif", 176, 144 ) );
  modes.append ( fetchFrameMode ( "cif", 352, 288 ) );
  modes.append ( fetchFrameMode ( "4cif", 704, 576 ) );
  modes.append ( fetchFrameMode ( "qqvga", 160, 120 ) );
  modes.append ( fetchFrameMode ( "qvga", 320, 240 ) );
  modes.append ( fetchFrameMode ( "vga", 640, 480 ) );
  modes.append ( fetchFrameMode ( "svga", 800, 600 ) );
  modes.append ( fetchFrameMode ( "xga", 1024, 768 ) );

  if ( maxWidth >= 1600 )
    modes.append ( fetchFrameMode ( "uxga", 1600, 1200 ) );

  if ( maxWidth >= 2048 )
    modes.append ( fetchFrameMode ( "qxga", 2048, 1536 ) );

  if ( maxWidth >= 1280 )
    modes.append ( fetchFrameMode ( "sxga", 1280, 1024 ) );

  if ( maxWidth >= 2560 )
    modes.append ( fetchFrameMode ( "qsxga", 2560, 2048 ) );

  if ( maxWidth >= 5120 )
    modes.append ( fetchFrameMode ( "hsxga", 5120, 4096 ) );

  if ( maxWidth >= 852 )
    modes.append ( fetchFrameMode ( "wvga", 852, 480 ) );

  if ( maxWidth >= 1366 )
    modes.append ( fetchFrameMode ( "wxga", 1366, 768 ) );

  if ( maxWidth >= 1600 )
    modes.append ( fetchFrameMode ( "wsxga", 1600, 1024 ) );

  if ( maxWidth >= 1920 )
    modes.append ( fetchFrameMode ( "wuxga", 1920, 1200 ) );

  if ( maxWidth >= 2560 )
    modes.append ( fetchFrameMode ( "woxga", 2560, 1600 ) );

  if ( maxWidth >= 3200 )
    modes.append ( fetchFrameMode ( "wqsxga", 3200, 2048 ) );

  if ( maxWidth >= 3840 )
    modes.append ( fetchFrameMode ( "wquxga", 3840, 2400 ) );

  if ( maxWidth >= 6400 )
    modes.append ( fetchFrameMode ( "whsxga", 6400, 4096 ) );

  if ( maxWidth >= 7680 )
    modes.append ( fetchFrameMode ( "whuxga", 7680, 4800 ) );

  modes.append ( fetchFrameMode ( "cga", 320, 200 ) );
  modes.append ( fetchFrameMode ( "ega", 640, 350 ) );
  modes.append ( fetchFrameMode ( "hd480", 852, 480 ) );

  if ( maxHeight > 1280 )
    modes.append ( fetchFrameMode ( "hd720", 1280, 720 ) );

  if ( maxHeight > 1920 )
    modes.append ( fetchFrameMode ( "hd1080", 1920, 1080 ) );

  if ( parent )
    modes.append ( grabScreenGeometry ( parent ) );

  return modes;
}

const FrameMode DesktopInfo::getFrameMode ( const QString &n, QWidget *parent )
{
  foreach ( FrameMode mode, modes( parent ) )
  {
    if ( mode.name == n )
      return mode;
  }
  return fetchFrameMode ( trUtf8 ( "Unknown" ), maxWidth, maxHeight );
}

const int DesktopInfo::getMaxWidth()
{
  return maxWidth;
}

const int DesktopInfo::getMaxHeight()
{
  return maxHeight;
}

const int DesktopInfo::getDepth()
{
  return Depth;
}

const FrameMode DesktopInfo::grabScreenGeometry ( QWidget *parent )
{
  QRect rect = desktopWidget->screenGeometry ( parent );
  if ( rect.isValid() )
  {
    QSize size = rect.size();
    maxWidth = size.width();
    maxHeight = size.height();
  }
  return fetchFrameMode ( trUtf8 ( "Fullscreen" ), maxWidth, maxHeight );
}

DesktopInfo::~DesktopInfo()
{
}



/***
* Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2008
* Copyright: See COPYING file that comes with this distribution
**/

#include "rubberband.h"
#include "desktopinfo.h"

#include <QtCore/QDebug>

RubberBand::RubberBand ( QWidget *parent )
    : QRubberBand ( QRubberBand::Rectangle, parent )
{
  setMinimumWidth ( 100 );
  setMinimumHeight ( 96 );
  m_DesktopInfo = new DesktopInfo ( this );

}

bool RubberBand::isScalability()
{
  if ( ( size().width() % 2 ) != 0 )
  {
    QString str = QString::number ( size().width() );
    emit error ( trUtf8 ( "Scalability Failure Width" ),
                 trUtf8 ( "Frame Width must be a multiple of 2" ) );
    return false;
  }

  if ( ( size().height() % 2 ) != 0 )
  {
    QString str = QString::number ( size().height() );
    emit error ( trUtf8 ( "Scalability Failure Height" ),
                 trUtf8 ( "Frame Height must be a multiple of 2" ) );
    return false;
  }

  return true;
}

RubberBand::~RubberBand()
{
}

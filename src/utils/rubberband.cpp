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

#include "rubberband.h"
#include "desktopinfo.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QPen>

RubberBand::RubberBand ( QWidget * parent )
    : QRubberBand ( QRubberBand::Rectangle, parent )
{
  setMinimumWidth ( 128 );
  setMinimumHeight ( 96 );
  setAutoFillBackground ( false );
  setContentsMargins ( 0, 0, 0, 0 );
  setAttribute ( Qt::WA_NoBackground, true );
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_PaintOnScreen, true );

  m_DesktopInfo = new DesktopInfo ( this );
}

void RubberBand::initStyleOption ( QStyleOptionRubberBand * option ) const
{
  if ( option )
    option->initFrom ( this );
}

void RubberBand::paintEvent ( QPaintEvent * event )
{
  QPainter painter ( this );
  painter.setBrush ( Qt::red );
  painter.setBackgroundMode ( Qt::TransparentMode );
  painter.setPen ( Qt::NoPen );
  painter.drawRect ( event->rect() );
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
{}

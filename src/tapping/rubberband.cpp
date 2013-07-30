/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "rubberband.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QLineF>
#include <QtCore/QPointF>
#include <QtCore/QSettings>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QStyle>
#include <QtGui/QStylePainter>

static const QPen defaultPenStyle()
{
  QBrush b ( Qt::red, Qt::SolidPattern );
  QPen p ( b, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin );
  return p;
}

RubberBand::RubberBand ( QWidget * parent )
    : QRubberBand ( QRubberBand::Line, parent )
    , p_pen ( defaultPenStyle() )
    , p_style ( QStyleOption::Version, QStyleOption::SO_RubberBand )
{
  setMinimumWidth ( 128 );
  setMinimumHeight ( 96 );
  setAutoFillBackground ( false );
  setContentsMargins ( 0, 0, 0, 0 );
  setWindowOpacity ( 1.0 ); // BUGFIX Composite extension behavior
  setAttribute ( Qt::WA_NoSystemBackground, true );
  setAttribute ( Qt::WA_SetPalette, false );
  setAttribute ( Qt::WA_SetStyle, false );
  setAttribute ( Qt::WA_OpaquePaintEvent, true );
  setAttribute ( Qt::WA_PaintOnScreen, true );
}

/**
* Ininitialsiere den Stylesheet erst bei einer neuen Ansicht!
*/
void RubberBand::showEvent ( QShowEvent * event )
{
  QColor c ( Qt::red );
  QSettings s ( QSettings::NativeFormat, QSettings::UserScope,
                qApp->organizationDomain(), qApp->applicationName() );
  c.setNamedColor ( s.value ( "Rubberband/Color", "#800000" ).toString() );

  p_pen.setColor ( c );
  p_pen.setWidth ( 1 );
  p_pen.setStyle ( Qt::SolidLine );
  QRubberBand::showEvent ( event );
}

/**
* Es gibt eingige Distribution die der Meinung sind sie müssten
* Unbedingt den Rubberband Style an Windoof anpassen. :-/
* Hier mein Hack um dieses wieder zu umgehen!
*/
void RubberBand::paintEvent ( QPaintEvent * _e )
{
  const QRect r = _e->rect();
  const QPointF p[8] =
  {
    QPointF ( r.topLeft() ),
    QPointF ( r.topRight() ),
    QPointF ( r.topRight() ),
    QPointF ( r.bottomRight() ),
    QPointF ( r.bottomRight() ),
    QPointF ( r.bottomLeft() ),
    QPointF ( r.bottomLeft() ),
    QPointF ( r.topLeft() )
  };

  QStylePainter sp ( this );
  sp.setCompositionMode ( QPainter::CompositionMode_DestinationOver );
  sp.setBackgroundMode ( Qt::TransparentMode );
  sp.setRenderHint ( QPainter::NonCosmeticDefaultPen, true );
  sp.setPen ( p_pen );
  sp.drawLines ( p, 4 );
  style()->drawControl ( QStyle::CE_RubberBand, &p_style, &sp, this );
}

bool RubberBand::isScalability()
{
  if ( ( size().width() & 1 ) || ( size().height() & 1 ) )
  {
    emit error ( trUtf8 ( "Scalability Failure " ),
                 trUtf8 ( "Frame Width/Height must be a multiple of 2" ) );
    return false;
  }
  return true;
}

RubberBand::~RubberBand()
{}

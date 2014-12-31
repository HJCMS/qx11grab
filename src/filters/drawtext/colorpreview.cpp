/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#include "colorpreview.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsEllipseItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPalette>
#include <QtGui/QPen>

ColorPreview::ColorPreview ( QWidget * parent )
    : QGraphicsView ( parent )
{
  setObjectName ( QLatin1String ( "ColorPreview" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMaximumSize ( 20, 20 );
  setCacheMode ( QGraphicsView::CacheNone );
  setAlignment ( ( Qt::AlignTop | Qt::AlignLeft ) );
  setInteractive ( true );
  setRenderHints ( QPainter::NonCosmeticDefaultPen );
  setTransformationAnchor ( QGraphicsView::NoAnchor );
  setViewportUpdateMode ( QGraphicsView::MinimalViewportUpdate );

  QBrush brush ( palette().color ( QPalette::Normal, QPalette::Window ) );
  brush.setStyle ( Qt::SolidPattern );
  setBackgroundBrush ( brush );

  // create a dummy scene for painter events
  setScene ( new QGraphicsScene ( this ) );
}

void ColorPreview::setColor ( const QColor &color )
{
  QBrush brush = backgroundBrush();
  brush.setColor ( color );
  setBackgroundBrush ( brush );
}

ColorPreview::~ColorPreview()
{}

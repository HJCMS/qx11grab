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
*/

#include "fontpreview.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRectF>
#include <QtCore/QPointF>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QFontMetrics>
#include <QtGui/QGraphicsBlurEffect>
#include <QtGui/QPalette>
#include <QtGui/QPainter>

FontPreview::FontPreview ( QWidget * parent )
    : QGraphicsView ( parent )
    , p_shadowOffset ( 2 )
{
  setObjectName ( QLatin1String ( "FontPreview" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setCacheMode ( QGraphicsView::CacheNone );
  setAlignment ( ( Qt::AlignTop | Qt::AlignLeft ) );
  setInteractive ( true );
  setRenderHints ( QPainter::TextAntialiasing );
  setTransformationAnchor ( QGraphicsView::NoAnchor );
  setViewportUpdateMode ( QGraphicsView::FullViewportUpdate );

  QPalette pal = palette();
  QBrush brush ( pal.color ( QPalette::Normal, QPalette::Window ) );
  brush.setStyle ( Qt::SolidPattern );
  setBackgroundBrush ( brush );

  m_graphicsScene = new QGraphicsScene ( this );
  setScene ( m_graphicsScene );

  m_graphicsTextShadowItem = m_graphicsScene->addText ( QString::fromUtf8 ( "QX11Grab" ), font() );
  QGraphicsBlurEffect* blur = new QGraphicsBlurEffect ( m_graphicsTextShadowItem );
  blur->setBlurHints ( QGraphicsBlurEffect::AnimationHint );

  m_graphicsTextItem = m_graphicsScene->addText ( QString::fromUtf8 ( "QX11Grab" ), font() );
  setShadowOffset ( p_shadowOffset );
}

void FontPreview::drawForeground ( QPainter * painter, const QRectF &rect )
{
  Q_UNUSED ( painter );
  Q_UNUSED ( rect );
//   qDebug() << Q_FUNC_INFO << text() << font() << rect;
}

void FontPreview::setText ( const QString &text )
{
  m_graphicsTextShadowItem->setFont ( font() );
  m_graphicsTextShadowItem->setPlainText ( text );
  m_graphicsTextItem->setFont ( font() );
  m_graphicsTextItem->setPlainText ( text );
  p_text = text;
}

void FontPreview::setBackgroundColor ( const QColor &color )
{
  QBrush brush = backgroundBrush();
  brush.setColor ( color );
  setBackgroundBrush ( brush );
}

void FontPreview::setTextColor ( const QColor &color )
{
  p_textColor = color;
  m_graphicsTextItem->setDefaultTextColor ( p_textColor );
}

void FontPreview::setShadowColor ( const QColor &color )
{
  p_shadowColor = color;
  m_graphicsTextShadowItem->setDefaultTextColor ( color );
}

void FontPreview::setShadowOffset ( int i )
{
  QPointF pos = m_graphicsTextItem->scenePos();
  pos.setX ( pos.x() + i );
  pos.setY ( pos.y() + i );

  m_graphicsTextShadowItem->setPos ( pos );
  p_shadowOffset = i;
}

QString FontPreview::text()
{
  return p_text;
}

const QColor FontPreview::backgroundColor()
{
  return backgroundBrush ().color();
}

const QColor FontPreview::textColor()
{
  return p_textColor;
}

const QColor FontPreview::shadowColor()
{
  return p_shadowColor;
}

int FontPreview::shadowOffset()
{
  return p_shadowOffset;
}

FontPreview::~FontPreview()
{}

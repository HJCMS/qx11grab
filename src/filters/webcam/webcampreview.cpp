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

#include "webcampreview.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QGraphicsAnchorLayout>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtGui/QImage>
#include <QtGui/QImageWriter>
#include <QtGui/QSizePolicy>

WebCamPreview::WebCamPreview ( QWidget * parent )
    : QGraphicsView ( parent )
    , m_pixmapItem ( 0 )
    , p_defaultSize ( QSize ( 160, 120 ) )
{
  setObjectName ( QLatin1String ( "WebCamPreview" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumSize ( 250, 250 );
  setDragMode ( QGraphicsView::NoDrag );
  // NOTE bestimmt das Koordinaten System!
  setAlignment ( ( Qt::AlignTop | Qt::AlignLeft ) );
  setInteractive ( true );
  setRenderHints ( QPainter::NonCosmeticDefaultPen );
  setTransformationAnchor ( QGraphicsView::NoAnchor );
  setViewportUpdateMode ( QGraphicsView::BoundingRectViewportUpdate );
  setOptimizationFlags ( QGraphicsView::DontAdjustForAntialiasing );
  setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
  setAttribute ( Qt::WA_PaintOnScreen, true );

  QBrush brush ( palette().color ( QPalette::Normal, QPalette::Mid ) );
  brush.setStyle ( Qt::CrossPattern );
  setBackgroundBrush ( brush );

  QPixmap dummyPixmap ( p_defaultSize );
  dummyPixmap.fill ( Qt::black );
  setScene ( new QGraphicsScene ( this ) );

  m_pixmapItem = scene()->addPixmap ( dummyPixmap );
}

void WebCamPreview::pixmapFromImage ( const QImage &image )
{
  if ( ! m_pixmapItem )
    return;

  QPixmap p = QPixmap::fromImage ( image, Qt::AutoColor );
  if ( ! p.isNull() )
    m_pixmapItem->setPixmap ( p );
}

void WebCamPreview::openWebCam()
{}

void WebCamPreview::closeWebCam()
{
  QPixmap p ( 150, 150 );
  p.fill ( Qt::black );
  m_pixmapItem->setPixmap ( p );
}

void WebCamPreview::setItemSize ( const QSizeF &size )
{
  QPixmap p = m_pixmapItem->pixmap().scaled ( size.toSize(), Qt::KeepAspectRatio );
  m_pixmapItem->setPixmap ( p );
  update();
}

const QSizeF WebCamPreview::itemSize()
{
  return m_pixmapItem->pixmap().size();
}

const QString WebCamPreview::device()
{
  return p_device;
}

void WebCamPreview::setDevice ( const QString &path )
{
  p_device = path;
}

WebCamPreview::~WebCamPreview()
{}

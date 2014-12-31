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
#include <QtGui/QMatrix>
#include <QtGui/QPalette>
#include <QtGui/QImage>
#include <QtGui/QImageWriter>
#include <QtGui/QSizePolicy>
#include <QtGui/QTransform>

WebCamPreview::WebCamPreview ( const QSizeF &baseSize, QWidget * parent )
    : QGraphicsView ( parent )
    , m_pixmapItem ( 0 )
    , p_itemSize ( baseSize )
{
  setObjectName ( QLatin1String ( "WebCamPreview" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumSize ( 280, 240 );
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

  setScene ( new QGraphicsScene ( this ) );

  // Erstelle einen schwarzen Frame für die Vorschau
  // NOTE Scene muss vorhanden sein!
  QPixmap dummyPixmap ( p_itemSize.toSize() );
  dummyPixmap.fill ( Qt::black );
  m_pixmapItem = scene()->addPixmap ( dummyPixmap );
  m_pixmapItem->setTransformationMode ( Qt::SmoothTransformation );
}

/**
* Erstellt das Vorschau Bild
*/
void WebCamPreview::setPreviewFrame ( const QImage &image )
{
  if ( ! m_pixmapItem )
  {
    qWarning ( "QX11Grab - Invalid v4l2 pixmap item - cancel scene update" );
    return;
  }

  QPixmap pixmap = QPixmap::fromImage ( image, Qt::AutoColor );
  QPixmap p = pixmap.scaled ( p_itemSize.toSize(), Qt::KeepAspectRatio );
  if ( ! p.isNull() )
    m_pixmapItem->setPixmap ( p );
}

/**
* Ändert den Skalierungsfaktor für den Frame und
* setzt gleichzeitg \ref p_itemSize Neu!
*/
void WebCamPreview::setFrameScale ( qreal s )
{
  m_pixmapItem->setScale ( s );
  p_itemSize = m_pixmapItem->sceneBoundingRect().size();
}

/**
* Anzeige zurück setzen...
*/
void WebCamPreview::restoreView()
{
  QPixmap dummyPixmap ( p_itemSize.toSize() );
  dummyPixmap.fill ( Qt::black );
  m_pixmapItem->setPixmap ( dummyPixmap );
}

/**
* Aktuelle Vorschaugröße
*/
const QSizeF WebCamPreview::itemSize()
{
  return p_itemSize;
}

WebCamPreview::~WebCamPreview()
{}

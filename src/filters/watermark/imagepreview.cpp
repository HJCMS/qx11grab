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

#include "imagepreview.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

/* QtGui */
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtGui/QImage>
#include <QtGui/QImageWriter>
#include <QtGui/QSizePolicy>

ImagePreview::ImagePreview ( QWidget * parent )
    : QGraphicsView ( parent )
    , m_pixmapItem ( 0 )
    , p_itemSize ( QSizeF ( 50, 50 ) )
{
  setObjectName ( QLatin1String ( "ImagePreview" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumSize ( 250, 250 );
//   setCacheMode ( QGraphicsView::CacheNone );
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
}

void ImagePreview::setItemOpacity ( int i )
{
  qreal opacity = static_cast<qreal> ( i );
  // qDebug() << Q_FUNC_INFO << ( opacity / 100 );
  if ( m_pixmapItem )
    m_pixmapItem->setOpacity ( ( opacity / 100 ) );
}

/**
* Füge ein neues Bild ein und setze \ref p_itemSize
* Wir können nicht die Dimensionen von der Scene nehmen weil
* der Rectangle sich auf den Sichtbaren Bereich bezieht.
* Bei einem herauslaufen des Bildes würde sich somit auch die
* Bildgröße verändern.
*/
bool ImagePreview::setImagePath ( const QString &path )
{
  QPixmap pixmap ( path, "PNG", Qt::AutoColor );
  scene()->clear();
  if ( ! pixmap.isNull() )
  {
    p_itemSize = QSizeF ( pixmap.size() );
    // qDebug() << Q_FUNC_INFO << p_itemRect;
    m_pixmapItem = scene()->addPixmap ( pixmap );
    return true;
  }
  return false;
}

/**
* Gibt die Bild größe zurück
*/
const QSizeF ImagePreview::itemSize()
{
  return p_itemSize;
}

/**
* Gibt die Bild Transparenz zurück
*/
const QString ImagePreview::opacity()
{
  return QString();
}

bool ImagePreview::savePixmap ( const QString &path )
{
  QImage image = m_pixmapItem->pixmap().toImage();
  image.convertToFormat ( QImage::Format_ARGB32_Premultiplied, Qt::ColorOnly );
  QString s = QString ( "%1x%2" ).arg ( QString::number ( image.width() ), QString::number ( image.height() ) );
  bool status = false;
  QFile fp ( path );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QImageWriter writer ( &fp, "png" );
    // writer.setCompression ();
    writer.setQuality ( 100 );
    float gamma = ( m_pixmapItem->opacity() >= 0.2 ) ? ( m_pixmapItem->opacity() - 0.1 ) : m_pixmapItem->opacity();
    writer.setGamma ( gamma );
    writer.setText ( "Description", "QX11Grab Watermark Filter tempfile" );
    writer.setText ( "Size", s );
    writer.setText ( "ScaledSize", s );
    writer.setText ( "Source", path );
    status = writer.write ( image );
    fp.close();
    return status;
  }
  return status;
}

ImagePreview::~ImagePreview()
{}

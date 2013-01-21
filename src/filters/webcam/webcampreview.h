/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#ifndef WEBCAMPREVIEW_H
#define WEBCAMPREVIEW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QWidget>

class WebCamPreview : public QGraphicsView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QGraphicsPixmapItem* m_pixmapItem;
    QSizeF p_itemSize;

  public Q_SLOTS:
    void setPreviewFrame ( const QImage & );
    void setFrameScale ( qreal s );
    void restoreView();

  public:
    explicit WebCamPreview ( const QSizeF &baseSize, QWidget * parent = 0 );

  const QSizeF itemSize();

    virtual ~WebCamPreview();
};

#endif

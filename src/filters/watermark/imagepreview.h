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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QSizeF>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>

class ImagePreview : public QGraphicsView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QGraphicsPixmapItem* m_pixmapItem;
    QSizeF p_itemSize;

  public Q_SLOTS:
    /** set image transparency */
    void setItemOpacity ( int i );

  public:
    explicit ImagePreview ( QWidget * parent = 0 );

    /** insert a new image an remove old image */
    bool setImagePath ( const QString &path );

    /** represents the current image size */
    const QSizeF itemSize();

    /** current image transparency */
    const QString opacity();

    /** current image */
    bool savePixmap ( const QString &path );

    ~ImagePreview();
};

#endif

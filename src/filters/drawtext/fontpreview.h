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

#ifndef FontPreview_H
#define FontPreview_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QRectF>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QPainter>
#include <QtGui/QWidget>

class FontPreview : public QGraphicsView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
    Q_PROPERTY ( QString text READ text WRITE setText )
    Q_PROPERTY ( QColor textColor READ textColor WRITE setTextColor )
    Q_PROPERTY ( QColor shadowColor READ shadowColor WRITE setShadowColor )
    Q_PROPERTY ( int shadowOffset READ shadowOffset WRITE setShadowOffset )

  private:
    QString p_text;
    QColor p_textColor;
    QColor p_shadowColor;
    int p_shadowOffset;
    QGraphicsScene* m_graphicsScene;
    QGraphicsTextItem* m_graphicsTextShadowItem;
    QGraphicsTextItem* m_graphicsTextItem;

  protected:
    virtual void drawForeground ( QPainter * painter, const QRectF &rect );

  public Q_SLOTS:
    /** set Text to Draw */
    void setText ( const QString &text );

    /** set Background Color */
    void setBackgroundColor ( const QColor &color );

    /** set Text Color */
    void setTextColor ( const QColor &color );

    /** set Shadow Text Color */
    void setShadowColor ( const QColor &color );

    /** set Shadow offset */
    void setShadowOffset ( int );

  public:
    explicit FontPreview ( QWidget * parent = 0 );

    /** get Drawed Text */
    QString text();

    /** get Background Color */
    const QColor backgroundColor();

    /** get Text Color */
    const QColor textColor();

    /** get Shadow Text Color */
    const QColor shadowColor();

    /** get Shadow offset */
    int shadowOffset();

    virtual ~FontPreview();
};

#endif

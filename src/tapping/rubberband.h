/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
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

#ifndef RUBBERBAND_H
#define RUBBERBAND_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QPaintEvent>
#include <QtGui/QPen>
#include <QtGui/QRubberBand>
#include <QtGui/QShowEvent>
#include <QtGui/QStyleOption>
#include <QtGui/QWidget>

class RubberBand : public QRubberBand
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Juergen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QPen p_pen;
    QStyleOption p_style;

  protected:
    /** init user configuration for border color */
    void showEvent ( QShowEvent * );

    /** update QStyle::CE_RubberBand */
    void paintEvent ( QPaintEvent * );

  Q_SIGNALS:
    void error ( const QString &, const QString & );

  public Q_SLOTS:
    /** a Hex Color Codes is required */
    void updateFrameColor ( const QString &hex );

  public:
    explicit RubberBand ( QWidget * parent = 0 );

    /** is it a scalable geometry */
    bool isScalability();

    virtual ~RubberBand();
};

#endif

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

#ifndef TEXTPOSITION_H
#define TEXTPOSITION_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class TextPosition : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )
    Q_PROPERTY ( int index READ index WRITE setIndex USER false )

  private:
    QString _x;
    QString _y;

    enum POSITION { UNKNOWN,
                    TOP_LEFT,
                    TOP_CENTER,
                    TOP_RIGHT,
                    MIDDLE_LEFT,
                    MIDDLE_CENTER,
                    MIDDLE_RIGHT,
                    BOTTOM_LEFT,
                    BOTTOM_CENTER,
                    BOTTOM_RIGTH
                };
    /** Position Selection */
    QComboBox* m_comboBox;

    /** text x indent in pixels. */
    QSpinBox* m_xIndent;

    /** text x indent in pixels. */
    QSpinBox* m_yIndent;

  private Q_SLOTS:
    void indexChanged ( int index );
    void indentChanged ( int );

  Q_SIGNALS:
    void postUpdate ();

  public Q_SLOTS:
    /** set position combo item index */
    void setIndex ( int );

  public:
    explicit TextPosition ( QWidget * parent = 0 );

    /** text position x */
    const QString x();

    /** text position y */
    const QString y();

    /** position combo item index */
    int index();

    ~TextPosition();
};

#endif

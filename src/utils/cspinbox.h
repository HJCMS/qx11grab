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

#ifndef CSPINBOX_H
#define CSPINBOX_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QAbstractSpinBox>
#include <QtGui/QColor>
#include <QtGui/QPaintEvent>
#include <QtGui/QSpinBox>
#include <QtGui/QValidator>
#include <QtGui/QWidget>

class CSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    const QColor invalid;
    const QColor normal;

  protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual QValidator::State validate ( QString & input, int & pos ) const;

  public:
    /**
    * spinbox with font color
    * if value is non-divisible it will paint a red text
    */
    explicit CSpinBox ( QWidget * parent = 0 );
    ~CSpinBox();
};

#endif

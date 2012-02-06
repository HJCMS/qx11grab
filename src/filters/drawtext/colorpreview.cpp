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

#include "colorpreview.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QFrame>

ColorPreview::ColorPreview ( QWidget * parent )
    : QLabel ( parent )
{
  setObjectName ( QLatin1String ( "ColorPreview" ) );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumWidth ( 15 );
  setIndent ( 0 );
  setBackgroundRole ( QPalette::WindowText );
  setBackgroundColor ( QColor ( 0, 0, 0 ) );
  setFrameStyle ( QFrame::Box );
}

void ColorPreview::setBackgroundColor ( const QColor &color )
{
  setStyleSheet ( QString::fromUtf8 ( "background:%1;" ).arg ( color.name() ) );
}

ColorPreview::~ColorPreview()
{}

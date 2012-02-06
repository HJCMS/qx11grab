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

#include "dropshadowbox.h"
#include "colorpreview.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QGridLayout>

DropShadowBox::DropShadowBox ( QWidget * parent )
    : QGroupBox ( parent )
{
  setObjectName ( QLatin1String ( "DropShadowBox" ) );

  QGridLayout* layout = new QGridLayout ( this );

  m_colorPreview = new ColorPreview ( this );
  layout->addWidget ( m_colorPreview, 0, 0, 1, 1 );

  m_buttonSetColor = new QToolButton ( this );
  m_buttonSetColor->setIcon ( QIcon::fromTheme ( "preferences-desktop-color" ) );
  layout->addWidget ( m_buttonSetColor, 0, 1, 1, 1 );

  m_shadowOffset = new QSlider ( Qt::Vertical, this );
  /*: ToolTip */
  m_shadowOffset->setToolTip ( trUtf8 ( "Shadow Offset" ) );
  m_shadowOffset->setRange ( 0, 100 );
  m_shadowOffset->setValue ( 2 );
  layout->addWidget ( m_shadowOffset, 1, 0, 1, 1 );

  m_shadowAlpha = new QSlider ( Qt::Vertical, this );
  /*: ToolTip */
  m_shadowAlpha->setToolTip ( trUtf8 ( "Shadow Alpha" ) );
  m_shadowAlpha->setRange ( 0, 255 );
  m_shadowAlpha->setValue ( 255 );
  layout->addWidget ( m_shadowAlpha, 1, 1, 1, 1 );

  setLayout ( layout );

  connect ( m_buttonSetColor, SIGNAL ( clicked () ),
            this, SIGNAL ( buttonClicked() ) );

  connect ( m_shadowOffset, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( offsetChanged ( int ) ) );

  connect ( m_shadowAlpha, SIGNAL ( valueChanged ( int ) ),
            this, SIGNAL ( alphaChanged ( int ) ) );
}

void DropShadowBox::setShadowColor ( const QColor &color )
{
  m_colorPreview->setBackgroundColor ( color );
}

DropShadowBox::~DropShadowBox()
{}

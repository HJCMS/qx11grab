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

#include "configrubberband.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

ConfigRubberband::ConfigRubberband ( QWidget * parent )
    : AbstractConfigWidget ( parent )
    , p_color ( Qt::red )
{
  setObjectName ( QLatin1String ( "ConfigRubberband" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "Rubberband Configuration" ) );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 5, 0, 0 );
  layout->setObjectName ( QLatin1String ( "ConfigRubberband/Layout" ) );

  m_colorWidget = new QColorDialog ( p_color, this );
  m_colorWidget->setWindowFlags ( Qt::Widget );
  m_colorWidget->setOption ( QColorDialog::NoButtons, true );
  m_colorWidget->setOption ( QColorDialog::ShowAlphaChannel, false );
  m_colorWidget->setOption ( QColorDialog::DontUseNativeDialog, true );
  layout->addWidget ( m_colorWidget );

  connect ( m_colorWidget, SIGNAL ( currentColorChanged ( const QColor & ) ),
            this, SLOT ( updateColor ( const QColor & ) ) );

  setLayout ( layout );
}

void ConfigRubberband::updateColor ( const QColor &color )
{
  if ( color.isValid() )
    p_color = color;
}

void ConfigRubberband::load ( Settings * cfg )
{
  p_color.setNamedColor ( cfg->value ( "Rubberband/Color", "#800000" ).toString() );
}

void ConfigRubberband::save ( Settings * cfg )
{
  cfg->setValue ( "Rubberband/Color", p_color.name() );
}

ConfigRubberband::~ConfigRubberband()
{}

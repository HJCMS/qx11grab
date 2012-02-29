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

#include "configextensions.h"
#include "extensiontable.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QVBoxLayout>

ConfigExtensions::ConfigExtensions ( QWidget * parent )
    : AbstractConfigWidget ( parent )
{
  setObjectName ( QLatin1String ( "ConfigExtensions" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "Configure Output Extensions" ) );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 5, 0, 0 );
  layout->setObjectName ( QLatin1String ( "ConfigExtensions/Layout" ) );

  m_extensionTable = new ExtensionTable ( this );
  layout->addWidget ( m_extensionTable );

  setLayout ( layout );
}

void ConfigExtensions::load ( Settings * cfg )
{
  m_extensionTable->load ( cfg );
}

void ConfigExtensions::save ( Settings * cfg )
{
  m_extensionTable->save ( cfg );
}

ConfigExtensions::~ConfigExtensions()
{}

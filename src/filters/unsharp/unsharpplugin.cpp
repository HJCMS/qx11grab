/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#include "unsharpplugin.h"
#include "unsharp.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QtPlugin>

bool UnsharpPlugin::create ( QWidget * parent )
{
  if ( parent )
  {
    m_unsharp = new Unsharp ( parent );
    if ( m_unsharp )
      return true;
  }
  m_unsharp = 0;
  return false;
}

bool UnsharpPlugin::exec()
{
  return ( m_unsharp->exec() == QDialog::Accepted ) ? true : false;
}

const QString UnsharpPlugin::pluginName()
{
  return QString::fromUtf8 ( "Unsharp" );
}

const QString UnsharpPlugin::title()
{
  return trUtf8 ( "Unsharp" );
}

const QString UnsharpPlugin::description()
{
  return trUtf8 ( "Sharpen or blur filter for the input" );
}

const QString UnsharpPlugin::data()
{
  QString val = m_unsharp->data();
  if ( val.isEmpty() )
    return QString::fromUtf8 ( "\"unsharp\"" );

  return val;
}

Q_EXPORT_PLUGIN2 ( Unsharp, UnsharpPlugin )

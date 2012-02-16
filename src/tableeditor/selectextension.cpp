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

#include "selectextension.h"
#include "optionsfinder.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
// #include <QtGui/QVBoxLayout>

SelectExtension::SelectExtension ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "SelectExtension" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "Output Extension" ) );
  /*: WhatsThis */
  setWhatsThis ( trUtf8 ( "Set Video Ouput Extension" ) );

  addItem ( trUtf8 ( "Extension" ), QString ( "avi" ) );

  setEnabled ( false );
}

void SelectExtension::insertItems ( const QString &codec )
{
  clear();

  OptionsFinder finder ( codec );
  QList<VideoExtension> list = finder.extensionList();
  if ( list.isEmpty() )
  {
    addItem ( trUtf8 ( "Extension" ), QString ( "avi" ) );
    setEnabled ( false );
    return;
  }

  setEnabled ( true );
  for ( int i = 0; i < list.size(); ++i )
  {
    VideoExtension e = list.at ( i );
    insertItem ( i, e.description, e.name );
    if ( e.isDefault )
      setCurrentIndex ( i );
  }
}

void SelectExtension::setDefault ( const QString &ext )
{
  if ( ( count() < 1 ) || ext.isEmpty() )
    return;

  for ( int i = 0; i < count(); ++i )
  {
    if ( itemData ( i, Qt::UserRole ).toString().compare ( ext ) == 0 )
    {
      setCurrentIndex ( i );
      break;
    }
  }
}

const QString SelectExtension::extension()
{
  return itemData ( currentIndex(), Qt::UserRole ).toString();
}

SelectExtension::~SelectExtension()
{}

/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#include "screencombobox.h"
#include "desktopinfo.h"

#include <QtCore/QVariant>

ScreenComboBox::ScreenComboBox ( QWidget *parent )
    : QComboBox ( parent )
{
  setObjectName( "screenComboBox" );
  setItems();

  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( setDataChanged ( int ) ) );
}

void ScreenComboBox::setItems()
{
  m_DesktopInfo = new DesktopInfo ( this );
  int index = 0;
  clear();

  foreach ( DesktopInfo::FrameMode mode, m_DesktopInfo->modes( this ) )
  {
    insertItem ( index, mode.summary );
    setItemData ( index, mode.name, Qt::UserRole );
    index++;
  }
}

void ScreenComboBox::setDataChanged ( int index )
{
  QString data = itemData ( index, Qt::UserRole ).toString();
  if ( data.isEmpty() )
    return;

  DesktopInfo::FrameMode mode = m_DesktopInfo->getFrameMode ( data, this );
  if ( mode.width < 100 )
    return;

  if ( mode.height < 90 )
    return;

  emit screenNameChanged ( data );
  emit screenWidthChanged ( mode.width );
  emit screenHeightChanged ( mode.height );
  emit screenDepthChanged ( mode.depth );
}

ScreenComboBox::~ScreenComboBox()
{
}

/*
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
*/

#include "screencombobox.h"
#include "desktopinfo.h"

#include <QtCore/QDebug>
#include <QtCore/QVariant>

ScreenComboBox::ScreenComboBox ( QWidget *parent )
    : QComboBox ( parent )
{
  setObjectName ( "screenComboBox" );
  // Daten müssen vor Signalen gesetzt sein
  m_DesktopInfo = new DesktopInfo ( this );
  setItems();

  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( setDataChanged ( int ) ) );
}

/**
* Alle Verfügbaren Rahmengrößen in die Auswahl einfügen.
*/
void ScreenComboBox::setItems()
{
  int index = 0;
  clear();

  foreach ( DesktopInfo::FrameMode mode, m_DesktopInfo->modes ( this ) )
  {
    insertItem ( index, mode.summary );
    setItemData ( index, mode.name, Qt::UserRole );
    index++;
  }
}

/**
* Verschiebe den Auswahl Index
* NOTE Wir Blockieren zu diesem Zeitpunkt die Signal Verarbeitung!
*/
void ScreenComboBox::moveToIndex ( int index )
{
  blockSignals ( true );
  setCurrentIndex ( index );
  blockSignals ( false );
}

/**
* Wird aufgerufen wenn der Benutzer einen Modus Manuell ausgewählt hat.
* @li Sucht nach dem passenden Modus
* @li Wenn kleiner als 100x90 dann aussteigen!
* @li Signale vergeben
*/
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

/**
* Sucht mit der Rahmen Geometrie nach einem Datensatz.
* Wenn gefunden wird @ref moveToIndex aufgerufen.
*/
void ScreenComboBox::setDataChanged ( const QRect &rect )
{
  QRect fRect ( 0, 0, rect.width(), rect.height() );
  DesktopInfo::FrameMode mode;
  for ( int i = 0; i < count(); ++i )
  {
    QString data = itemData ( i, Qt::UserRole ).toString();
    mode = m_DesktopInfo->getFrameMode ( data, this );
    QRect mRect ( 0, 0, mode.width, mode.height );
    if ( mRect == fRect )
    {
      moveToIndex ( i );
      break;
    }
  }
}

ScreenComboBox::~ScreenComboBox()
{
}

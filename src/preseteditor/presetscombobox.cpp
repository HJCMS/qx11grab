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

#include "presetscombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QIcon>

/**
* @class PresetsComboBox
* Auswahl Box für alle (Benutzer/System) ffpresets
*/
PresetsComboBox::PresetsComboBox ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "PresetsComboBox" ) );
  setInsertPolicy ( QComboBox::InsertAtBottom );
  setMaxVisibleItems ( 15 );
  insertItem ( 0, QIcon::fromTheme ( "document-properties" ), trUtf8 ( "Select Preset" ), QString() );
  load();

  connect ( this, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( indexChanged ( int ) ) );
}

/**
* Erstellt einen ComboBox Eintrag.
*/
void PresetsComboBox::insertItemData ( const QFileInfo &info )
{
  QIcon icon;
  int index = ( count() + 1 );
  if ( info.isWritable() )
    icon = QIcon::fromTheme ( "document-properties" );
  else
    icon = QIcon::fromTheme ( "document-encrypt" );

  insertItem ( index, icon, info.completeBaseName(), info.absoluteFilePath() );
}

/**
* Daten einlesen und an @ref insertItemData übergeben.
*/
void PresetsComboBox::load()
{
  QStringList nameFilters ( "*.ffpreset" );
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "%1/.ffmpeg" ).arg ( d.homePath() ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    insertItemData ( info );
  }
  d.setPath ( QString::fromUtf8 ( "/usr/share/ffmpeg" ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    insertItemData ( info );
  }
  setCurrentIndex ( 0 );
}

/**
* Ein Index wurde ausgewählt.
* Ist dieser mit einem lesbaren Preset verbunden
* wird das signal @ref fileChanged aufgerufen.
*/
void PresetsComboBox::indexChanged ( int i )
{
  QString file = itemData ( i, Qt::UserRole ).toString();
  if ( ! file.isEmpty() )
    emit fileChanged ( file );
}

/**
* Leert die ComboBox und liest alles neu ein!
*/
void PresetsComboBox::reload()
{
  clear();
  insertItem ( 0, QIcon::fromTheme ( "document-properties" ), trUtf8 ( "Select Preset" ), QString() );
  load();
}

PresetsComboBox::~PresetsComboBox()
{
  clear();
}

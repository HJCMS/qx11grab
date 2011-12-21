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

#include "selectpresets.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

SelectPresets::SelectPresets ( QWidget * parent )
    : AbstractSelection ( parent )
    , nameFilters ( QStringList ( "*.ffpreset" ) )
{
  setObjectName ( QLatin1String ( "SelectPresets" ) );
  setToolTip ( trUtf8 ( "For the vpre, apre, and spre options, the options specified in a preset file are applied to the currently selected codec of the same type as the preset option." ) );
  reload();
}

const QStringList SelectPresets::userPresets()
{
  QStringList list;
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "%1/.ffmpeg" ).arg ( d.homePath() ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    list.append ( info.completeBaseName() );
  }
  return list;
}

const QStringList SelectPresets::systemPresets()
{
  QStringList list;
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "/usr/share/ffmpeg" ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    list.append ( info.completeBaseName() );
  }
  return list;
}

void SelectPresets::reload()
{
  QStringList list;
  list << userPresets() << systemPresets();
  clear();
  addItem ( trUtf8 ( "Presets" ), false );
  foreach ( QString val,  list )
  {
    addItem ( val, true );
  }
}

SelectPresets::~SelectPresets()
{}

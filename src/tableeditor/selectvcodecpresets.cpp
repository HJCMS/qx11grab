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

#include "selectvcodecpresets.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

SelectVcodecPresets::SelectVcodecPresets ( QWidget * parent )
    : AbstractSelection ( parent )
    , nameFilters ( QStringList ( "*.ffpreset" ) )
{
  setObjectName ( QLatin1String ( "SelectVcodecPresets" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "For the vpre, apre, and spre options, the options specified in a preset file are applied to the currently selected codec of the same type as the preset option." ) );

  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  QDBusReply<QString> reply = iface.call ( "getVideoCodec" );
  if ( reply.isValid() )
    codecSuffix = reply.value();

  reload();
}

void SelectVcodecPresets::initItemDataset()
{}

const QStringList SelectVcodecPresets::userPresets ( const QString &suffix )
{
  QStringList list;
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "%1/.ffmpeg" ).arg ( d.homePath() ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    QString bn = info.completeBaseName();
    QRegExp pattern ( "^"+suffix+"\\-" );
    if ( bn.contains ( pattern ) )
    {
      list.append ( bn.replace ( pattern, "" ) );
    }
  }
  return list;
}

const QStringList SelectVcodecPresets::systemPresets ( const QString &suffix )
{
  QStringList list;
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "/usr/share/ffmpeg" ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    QString bn = info.completeBaseName();
    QRegExp pattern ( "^"+suffix+"\\-" );
    if ( bn.contains ( pattern ) )
    {
      list.append ( bn.replace ( pattern, "" ) );
    }
  }
  return list;
}

void SelectVcodecPresets::reload()
{
  QStringList list;
  if ( ! codecSuffix.isEmpty() )
    list << userPresets ( codecSuffix ) << systemPresets ( codecSuffix );

  clear();
  addItem ( trUtf8 ( "Presets for (%1)" ).arg ( codecSuffix ), false );
  foreach ( QString val,  list )
  {
    addItem ( val, true );
  }
}

SelectVcodecPresets::~SelectVcodecPresets()
{}

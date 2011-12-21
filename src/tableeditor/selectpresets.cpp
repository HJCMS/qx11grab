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

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

SelectPresets::SelectPresets ( QWidget * parent )
    : AbstractSelection ( parent )
    , nameFilters ( QStringList ( "*.ffpreset" ) )
{
  setObjectName ( QLatin1String ( "SelectPresets" ) );
  setToolTip ( trUtf8 ( "For the vpre, apre, and spre options, the options specified in a preset file are applied to the currently selected codec of the same type as the preset option." ) );

  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );

  QDBusReply<QString> reply = iface.call ( QLatin1String ( "getVideoCodec" ) );
  if ( reply.isValid() )
    videoCodec = reply.value();

  reply = iface.call ( QLatin1String ( "getAudioCodec" ) );
  if ( reply.isValid() )
    audioCodec = reply.value();

  reload();
}

const QStringList SelectPresets::userPresets ( const QString &suffix )
{
  QStringList list;
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "%1/.ffmpeg" ).arg ( d.homePath() ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    QString bn = info.completeBaseName();
    if ( bn.contains ( suffix ) )
    {
      list.append ( bn.replace ( QRegExp ( "^"+suffix+"\\-" ), "" ) );
    }
  }
  return list;
}

const QStringList SelectPresets::systemPresets ( const QString &suffix )
{
  QStringList list;
  QDir d ( QDir::home() );
  d.setPath ( QString::fromUtf8 ( "/usr/share/ffmpeg" ) );
  foreach ( QFileInfo info, d.entryInfoList ( nameFilters, QDir::Files, QDir::Name ) )
  {
    QString bn = info.completeBaseName();
    if ( bn.contains ( suffix ) )
    {
      list.append ( bn.replace ( QRegExp ( "^"+suffix+"\\-" ), "" ) );
    }
  }
  return list;
}

void SelectPresets::reload()
{
  QStringList list;
  if ( ! videoCodec.isEmpty() )
    list << userPresets ( videoCodec ) << systemPresets ( videoCodec );

  if ( ! audioCodec.isEmpty() )
    list << userPresets ( audioCodec ) << systemPresets ( audioCodec );

  clear();
  addItem ( trUtf8 ( "Presets for (%1/%2)" ).arg ( videoCodec, audioCodec ), false );
  foreach ( QString val,  list )
  {
    addItem ( val, true );
  }
}

SelectPresets::~SelectPresets()
{}
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

#include "selectacodecpresets.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

/* QX11Grab */
#include "avoptions.h"

SelectAcodecPresets::SelectAcodecPresets ( QWidget * parent )
    : AbstractSelection ( parent )
{
  setObjectName ( QLatin1String ( "SelectAcodecPresets" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "For the vpre, apre, and spre options, the options specified in a preset file are applied to the currently selected codec of the same type as the preset option." ) );

  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  QDBusReply<QString> reply = iface.call ( "audiocodec" );
  if ( reply.isValid() )
    codecSuffix = reply.value();
  else
    qDebug() << Q_FUNC_INFO << reply.error().name() << reply.error().message();
  
  reload();
}

void SelectAcodecPresets::initItemDataset()
{}

void SelectAcodecPresets::reload()
{
  QStringList list;
  if ( ! codecSuffix.isEmpty() )
  {
    list << QX11Grab::AVOptions::userPresets ( codecSuffix );
    list << QX11Grab::AVOptions::systemPresets ( codecSuffix );
  }

#ifdef MAINTAINER_REPOSITORY
    qDebug() << Q_FUNC_INFO << list;
#endif

  clear();
  addItem ( trUtf8 ( "Presets for (%1)" ).arg ( codecSuffix ), false );
  foreach ( QString val,  list )
  {
    addItem ( val, true );
  }
}

SelectAcodecPresets::~SelectAcodecPresets()
{}

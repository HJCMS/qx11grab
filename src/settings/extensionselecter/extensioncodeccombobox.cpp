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

#include "extensioncodeccombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>

/* QX11Grab */
#include "avoptions.h"

ExtensionCodecComboBox::ExtensionCodecComboBox ( QWidget * parent )
    : QX11Grab::CodecSelecter ( parent )
{
  setObjectName ( QLatin1String ( "ExtensionCodecComboBox" ) );
  QList<QX11Grab::FFCodec> list = QX11Grab::AVOptions::videoCodecs();
  if ( list.size() > 0 )
    setCodecItems ( list );
}

ExtensionCodecComboBox::~ExtensionCodecComboBox()
{}

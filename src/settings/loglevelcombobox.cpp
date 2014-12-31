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

#include "loglevelcombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>

LogLevelComboBox::LogLevelComboBox ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "LogLevelComboBox" ) );
  /*: ToolTip */
  setToolTip ( trUtf8 ( "FFmpeg logging level" ) );
  /*: WhatsThis */
  setWhatsThis ( trUtf8 ( "Set the logging level used by the FFmpeg application" ) );

  int index = 0;
  QIcon icon = QIcon::fromTheme ( "menu-debugger" );
  insertItem ( index++, icon, trUtf8 ( "Warnings" ), QString::fromUtf8 ( "warning" ) );
  insertItem ( index++, icon, trUtf8 ( "Information" ), QString::fromUtf8 ( "info" ) );
  insertItem ( index++, icon, trUtf8 ( "Verbose" ), QString::fromUtf8 ( "verbose" ) );
  insertItem ( index++, icon, trUtf8 ( "Errors" ), QString::fromUtf8 ( "error" ) );
  insertItem ( index++, icon, trUtf8 ( "Panic" ), QString::fromUtf8 ( "panic" ) );
  insertItem ( index++, icon, trUtf8 ( "Fatal" ), QString::fromUtf8 ( "fatal" ) );
  insertItem ( index++, icon, trUtf8 ( "Manual" ), QString::fromUtf8 ( "NONE" ) );
  setCurrentIndex ( 0 );
}

void LogLevelComboBox::setValue ( const QString &value )
{
  for ( int i = 0; i < count(); ++i )
  {
    if ( itemData ( i, Qt::UserRole ).toString().compare ( value ) == 0 )
      setCurrentIndex ( i );
  }
}

const QString LogLevelComboBox::value()
{
  QString item = itemData ( currentIndex(), Qt::UserRole ).toString();
  return ( item.compare ( "NONE" ) == 0 ) ? QString() : item;
}

LogLevelComboBox::~LogLevelComboBox()
{}

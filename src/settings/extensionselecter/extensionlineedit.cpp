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

#include "extensionlineedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QCompleter>
#include <QtGui/QRegExpValidator>

ExtensionLineEdit::ExtensionLineEdit ( QWidget * parent )
    : QLineEdit ( parent )
{
  setObjectName ( QLatin1String ( "ExtensionLineEdit" ) );
  QRegExpValidator* mask = new QRegExpValidator ( this );
  mask->setRegExp ( QRegExp ( "^[\\w\\d]+$" ) );
  setValidator ( mask );
}

void ExtensionLineEdit::insertCompleter ( CodecID id )
{
  if ( id == CODEC_ID_NONE )
    return;

  QStringList buffer;
  QList<QX11Grab::FFFormat> list = QX11Grab::AVOptions::supportedFormats ( id );
  if ( list.size() > 0 )
  {
    for ( int i = 0; i < list.size(); ++i )
    {
      buffer.append ( list.at ( i ).extensions.toStringList() );
    }
  }

  if ( buffer.isEmpty() )
    return;

  QCompleter* cpl = new QCompleter ( buffer, this );
  setCompleter ( cpl );
  buffer.clear();
}

void ExtensionLineEdit::setCompleterCodec ( const QString &codec )
{
  QList<QX11Grab::FFCodec> list = QX11Grab::AVOptions::videoCodecs();
  if ( list.size() > 0 )
  {
    // erst nach der ID Suchen
    for ( int i = 0; i < list.size(); ++i )
    {
      // Bei einem Treffer befüllen
      if ( list.at ( i ).name.compare ( codec ) == 0 )
      {
        insertCompleter ( list.at ( i ).id );
        break;
      }
    }
  }
}

ExtensionLineEdit::~ExtensionLineEdit()
{}

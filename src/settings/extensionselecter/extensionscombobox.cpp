/*
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
*/

#include "extensionscombobox.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QIcon>

ExtensionsComboBox::ExtensionsComboBox ( QWidget * parent )
    : QComboBox ( parent )
{
  setObjectName ( QLatin1String ( "ExtensionsComboBox" ) );
  setDuplicatesEnabled ( false );
  setEditable ( true );
  setInsertPolicy ( QComboBox::InsertAtCurrent );
  // Siehe updateItemList
  insertItem ( 0, QString(), QVariant ( false ) );
  setItemIcon ( 0, QIcon::fromTheme ( "edit-rename" ) );

  connect ( this, SIGNAL ( editTextChanged ( const QString & ) ),
            this, SLOT ( updateItemList ( const QString & ) ) );
}

/**
* Lese alle Erweterungen an Hand der CodecID ein und
* erstelle das Auswahl Menü!
* TODO Alternativen aus Settings lesen!
*/
void ExtensionsComboBox::insertExtensions ( CodecID id )
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
  addItems ( buffer );
  buffer.clear();
}

/**
* Der User hat eine Benutzerdefinierte Erweiterung eingefügt
*/
void ExtensionsComboBox::updateItemList ( const QString &text )
{
  int index = currentIndex();
  if ( text.isEmpty() && itemData ( index ).toBool() )
    removeItem ( index );
}

/**
* Setze den ausgewählten Codec
* Hiermit wir der Standard Datensatz definiert!
*/
void ExtensionsComboBox::setCodec ( const QString &codec )
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
        insertExtensions ( list.at ( i ).id );
        break;
      }
    }
  }
}

/**
* Erwartet eine Komma(,) Deliminierte Zeichenkette!
* Für das Konvertieren in eine String Liste.
*/
void ExtensionsComboBox::setText ( const QString &txt )
{
  QStringList list = txt.split ( "," );
  for ( int i = 0; i < list.size(); ++i )
  {
    QString buffer = list.at ( i ).trimmed();
    if ( ! buffer.isEmpty() )
      addItem ( buffer, QVariant ( true ) );
  }
}

/**
* Aktuelle Erweiterungen mit zurück geben!
*/
const QStringList ExtensionsComboBox::data()
{
  QStringList list;
  for ( int i = 0; i < count(); ++i )
  {
    QString buffer = itemText ( i ).trimmed();
    if ( ! buffer.isEmpty() )
      list.append ( buffer );
  }
  return list;
}

ExtensionsComboBox::~ExtensionsComboBox()
{}

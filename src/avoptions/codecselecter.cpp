/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "codecselecter.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QIcon>

namespace QX11Grab
{
  /**
  * Auswahl Box Klasse zum setzen des Verfügbaren Kodierers
  */
  CodecSelecter::CodecSelecter ( QWidget * parent )
      : QComboBox ( parent )
  {
    setObjectName ( QLatin1String ( "CodecSelecter" ) );
    setEditable ( true ); // Benutzer kann neue Einträge erzeugen!
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( itemSelected ( int ) ) );
  }

  /**
  * Ein Menü Eintrag wurde ausgewählt.
  * \li Suche nach der FFmpeg \ref AVCodecID
  * \li Sende bei erfolg das Signal \ref codecChanged
  */
  void CodecSelecter::itemSelected ( int index )
  {
    QString name = itemData ( index, Qt::DisplayRole ).toString();
    bool ok;
    AVCodecID id = static_cast<AVCodecID> ( itemData ( index, Qt::UserRole ).toUInt ( &ok ) );
    if ( ok )
      emit codecChanged ( name, id );
  }

  /**
  * Sucht den Kodierer in den Einträgen mit dem Namen.
  * \li Sucht den angezeigten Kodierer Namen,
  * \li Wenn gefunden markiert ihn als ausgewählt,
  * \li Sendet über \ref itemSelected das Signal \ref codecChanged
  */
  void CodecSelecter::setCodec ( const QString &name )
  {
    int index = findData ( name, Qt::DisplayRole, ( Qt::MatchExactly | Qt::MatchCaseSensitive ) );
    if ( index != -1 )
    {
      setCurrentIndex ( index );
      // NOTE wird by onLoad für FormatMenu benötigt!
      itemSelected ( index );
    }
  }

  /**
  * Alle verfügbaren Codecs einfügen
  */
  void CodecSelecter::setCodecItems ( const QList<QX11Grab::FFCodec> &list )
  {
    int index = 0;
    clear();

    for ( int i = 0; i < list.size(); ++i )
    {
      QX11Grab::FFCodec codec = list.at ( i );
      insertItem ( index, codec.name, QVariant ( codec.name ) );
      setItemData ( index, codec.name, Qt::DisplayRole );
      setItemData ( index, codec.name, Qt::EditRole );
      setItemData ( index, codec.id, Qt::UserRole );
      setItemData ( index, codec.fullname, Qt::ToolTipRole );
      index = i;
    }
  }

  /**
  * Gibt dem Benutzer die Möglichkeit zusätzliche Kodierer ein zu fügen.
  * \li Suche nach Vorhandenen Einträgen,
  * \li Wenn vorhanden - Markieren und aussteigen,
  * \li Andernfalls neuen Eintrag einfügen.
  * \note Neue Einträge werden mit \ref AV_CODEC_ID_NONE eingefügt
  *       und senden deshalb bei einer Auswahl kein Signal!
  */
  void CodecSelecter::setCustomItem ( const QString &key, const QVariant &value )
  {
    int c = count();
    for ( int i = 0; i < c; ++i )
    {
      if ( itemData ( i, Qt::DisplayRole ).toString().compare ( key ) == 0 )
      {
        setCurrentIndex ( i );
        return;
      }
    }

    insertItem ( c, key, value );
    setItemData ( c, key, Qt::DisplayRole );
    setItemData ( c, key, Qt::EditRole );
    setItemData ( c, AV_CODEC_ID_NONE, Qt::UserRole );
    /*: ToolTip */
    setItemData ( c, trUtf8 ( "Customized" ), Qt::ToolTipRole );
  }

  /**
  * Aktuellen Codec Text ausgeben
  */
  const QString CodecSelecter::getCodec ()
  {
    return itemData ( currentIndex(), Qt::DisplayRole ).toString();
  }

  CodecSelecter::~CodecSelecter()
  {}
}  /* eof namespace QX11Grab */

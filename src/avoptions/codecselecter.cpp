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

#include "codecselecter.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QIcon>

namespace QX11Grab
{
  CodecSelecter::CodecSelecter ( QWidget * parent )
      : QComboBox ( parent )
  {
    setObjectName ( QLatin1String ( "CodecSelecter" ) );
    setEditable ( true );
    connect ( this, SIGNAL ( activated ( int ) ),
              this, SLOT ( itemSelected ( int ) ) );
  }

  void CodecSelecter::itemSelected ( int index )
  {
    QString name = itemData ( index, Qt::DisplayRole ).toString();
    bool ok;
    CodecID id = static_cast<CodecID> ( itemData ( index, Qt::UserRole ).toUInt ( &ok ) );
    if ( ok )
      emit codecChanged ( name, id );
  }

  void CodecSelecter::setCodec ( const QString &name )
  {
    int index = findData ( name, Qt::DisplayRole, ( Qt::MatchExactly | Qt::MatchCaseSensitive ) );
    if ( index != -1 )
    {
      setCurrentIndex ( index );
      itemSelected ( index ); // NOTE wird by onLoad für FormatMenu benötigt!
    }
  }

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

  void CodecSelecter::setCustomItem ( const QString &key, const QVariant &value )
  {
    int i = count();
    insertItem ( i, key, value );
    setItemData ( i, key, Qt::DisplayRole );
    setItemData ( i, key, Qt::EditRole );
    setItemData ( i, CODEC_ID_NONE, Qt::UserRole );
    /*: ToolTip */
    setItemData ( i, trUtf8 ( "Customized" ), Qt::ToolTipRole );
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

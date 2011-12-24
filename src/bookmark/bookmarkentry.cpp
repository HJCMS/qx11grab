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

#include "bookmarkentry.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

BookmarkEntry::BookmarkEntry ( QDomElement &rootNode )
    : QDomElement ( rootNode )
{}

void BookmarkEntry::initDefaults()
{
  QDomDocument doc = ownerDocument();
  vcodecNode = doc.createElement ( "vcodec" );
  appendChild ( vcodecNode );

  acodecNode = doc.createElement ( "acodec" );
  appendChild ( acodecNode );
}

const QString BookmarkEntry::getCodecName ( TYPE type )
{
  if ( type == VCODEC )
    return vcodecNode.attribute ( "id" );
  else if ( type == ACODEC )
    return acodecNode.attribute ( "id" );

  return QString();
}

void BookmarkEntry::setCodecOptions ( TYPE t, const QString &codecName, const QHash<QString,QVariant> &hash )
{
  QDomDocument doc = ownerDocument();
  QString id = codecName.trimmed();

  if ( ! hasAttribute ( "title" ) )
    qFatal ( "empty title attributes" );

  if ( id.isEmpty() )
  {
    if ( t == VCODEC )
      removeChild ( vcodecNode );
    else
      removeChild ( acodecNode );

    return;
  }

  QString nodeName = ( t == VCODEC ) ? "vcodec" : "acodec";
  QDomElement sharedNode = doc.createElement ( nodeName );
  sharedNode.setAttribute ( "id", id );

  QHash<QString,QVariant> h = hash;
  QHash<QString,QVariant>::iterator it;
  for ( it = h.begin(); it != h.end(); ++it )
  {
    QDomElement e = doc.createElement ( "param" );
    sharedNode.appendChild ( e );

    QDomElement a = doc.createElement ( "argument" );
    a.appendChild ( doc.createCDATASection ( it.key() ) );
    e.appendChild ( a );
    QDomElement v = doc.createElement ( "value" );
    v.appendChild ( doc.createCDATASection ( it.value().toString() ) );
    e.appendChild ( v );
  }

  /** Suche nach Existierenden Einträgen
  * Wenn Element mit id vorhanden existieren dann ersetzen.
  * Andernfalls füge den Neuen Elemente-Baum ein.
  */
  if ( t == VCODEC )
  {
    if ( vcodecNode.attribute ( "id" ).compare ( id ) == 0 )
    {
      if ( ! replaceChild ( sharedNode, vcodecNode ).isNull() )
        vcodecNode = sharedNode;
    }
    else
      vcodecNode.appendChild ( sharedNode );
  }
  else if ( t == ACODEC )
  {
    if ( acodecNode.attribute ( "id" ).compare ( id ) == 0 )
    {
      if ( ! replaceChild ( sharedNode, acodecNode ).isNull() )
        acodecNode = sharedNode;
    }
    else
      acodecNode.appendChild ( sharedNode );
  }
}

const QHash<QString,QVariant> BookmarkEntry::getCodecOptions ( TYPE t )
{
  QHash<QString,QVariant> hash;
  QDomElement codecNode;
  if ( t == VCODEC )
    codecNode = vcodecNode;
  else if ( t == ACODEC )
    codecNode = acodecNode;
  else
    return hash;

  if ( codecNode.hasChildNodes() )
  {
    QDomNodeList nodes = codecNode.elementsByTagName ( "param" );
    for ( int n = 0; n < nodes.size(); ++n )
    {
      QDomElement e = nodes.item ( n ).toElement();
      if ( e.hasChildNodes() )
      {
        hash.insert ( e.firstChildElement ( "argument" ).firstChild().nodeValue(),
                      e.firstChildElement ( "value" ).firstChild().nodeValue() );
      }
    }
  }
  return hash;
}

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

#include "bookmarkentry.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtXml */
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

BookmarkEntry::BookmarkEntry ( const QDomElement &entry )
    : QDomElement ( entry )
    , doc ( entry.ownerDocument() )
{}

BookmarkEntry& BookmarkEntry::operator= ( const BookmarkEntry &p )
{
  return static_cast<BookmarkEntry&> ( QDomElement::operator= ( p ) );
}

/** prüft ob das element vorhanden ist! */
bool BookmarkEntry::elementExists ( const QString &nodeName )
{
  QDomElement e = firstChildElement ( nodeName );
  // qDebug() << Q_FUNC_INFO << "SEARCH:" << nodeName << " FROM:" << attribute ( "title" );
  if ( e.isNull() )
    return false;
  else if ( e.nodeType() != QDomNode::ElementNode )
    return false;

  return true;
}

/** Suche nach Element vcodec */
QDomElement BookmarkEntry::vcodecNode()
{
  if ( ! elementExists ( "vcodec" ) )
  {
    QDomElement element = doc.createElement ( "vcodec" );
    appendChild ( element );
  }
  return firstChildElement ( "vcodec" );
}

/** Suche nach Element acodec */
QDomElement BookmarkEntry::acodecNode()
{
  if ( ! elementExists ( "acodec" ) )
  {
    QDomElement element = doc.createElement ( "acodec" );
    appendChild ( element );
  }
  return firstChildElement ( "acodec" );
}

/** Schreibe oder ersetze Einträge in "vcodec" */
void BookmarkEntry::setVCodecChildNodes ( const QDomElement &node )
{
  QDomNode oldNode = vcodecNode();
  if ( replaceChild ( node, oldNode ).isNull() )
  {
    qWarning ( "<%s title=\"%s\" /> can not replace vcodec node failed",
               qPrintable ( nodeName() ),
               qPrintable ( attribute ( "title" ) ) );
  }
}

/** Schreibe oder ersetze Einträge in "acodec" */
void BookmarkEntry::setACodecChildNodes ( const QDomElement &node )
{
  QDomNode oldNode = acodecNode();
  if ( replaceChild ( node, oldNode ).isNull() )
  {
    qWarning ( "<%s title=\"%s\" /> can not replace acodec node failed",
               qPrintable ( nodeName() ),
               qPrintable ( attribute ( "title" ) ) );
  }
}

/**
* Wenn Prädikat \b title vorhanden ist
* und Unter Elemente existieren.
* Wird true zurück gegeben!
*/
bool BookmarkEntry::isValid()
{
  if ( ( nodeType() == QDomNode::ElementNode ) && hasAttribute ( "title" ) )
    return true;

  return false;
}

/**
* Aktueller Codec Titel für Element Type
*/
const QString BookmarkEntry::getCodecName ( BTYPE type )
{
  QString name;
  if ( type == VCODEC )
    name = vcodecNode().attribute ( "id" );
  else if ( type == ACODEC )
    name = acodecNode().attribute ( "id" );

  return name;
}

/**
* Schreibe oder ersetze Parameterliste in Codec Type
*/
void BookmarkEntry::setCodecOptions ( BTYPE type, const QString &codecName, const QHash<QString,QVariant> &hash )
{
  // qDebug() << Q_FUNC_INFO << codecName;
  if ( ! hasAttribute ( "title" ) )
    qFatal ( "empty title attributes" );

  QString id = codecName.trimmed();
  if ( id.isEmpty() )
  {
    qWarning ( "no codec param id found - dropout" );
    return;
  }

  QString nodeName = ( type == VCODEC ) ? "vcodec" : "acodec";
  QDomElement sharedNode = doc.createElement ( nodeName );
  sharedNode.setAttribute ( "id", id );

  QHash<QString,QVariant> h = hash;
  QHash<QString,QVariant>::iterator it;
  for ( it = h.begin(); it != h.end(); ++it )
  {
    QDomElement param = doc.createElement ( "param" );
    sharedNode.appendChild ( param );

    QDomElement a = doc.createElement ( "argument" );
    a.appendChild ( doc.createCDATASection ( it.key() ) );
    param.appendChild ( a );
    QDomElement v = doc.createElement ( "value" );
    v.appendChild ( doc.createCDATASection ( it.value().toString() ) );
    param.appendChild ( v );
  }

  if ( type == VCODEC )
    setVCodecChildNodes ( sharedNode );
  else if ( type == ACODEC )
    setACodecChildNodes ( sharedNode );
}

/**
* Aktuelle Codec Parameter von Type auslesen
*/
const QHash<QString,QVariant> BookmarkEntry::getCodecOptions ( BTYPE type )
{
  QHash<QString,QVariant> hash;
  QDomElement codecNode;
  if ( ( type == VCODEC ) && elementExists ( "vcodec" ) )
    codecNode = vcodecNode();
  else if ( ( type == ACODEC ) && elementExists ( "acodec" ) )
    codecNode = acodecNode();
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

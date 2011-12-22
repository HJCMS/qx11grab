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

#include "bookmark.h"

/* QtCore */
#include <QtCore/QDate>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtXml */
#include <QtXml/QDomNodeList>
#include <QtXml/QDomText>

static const QString bookmarkDataPath()
{
  QString dest = QDesktopServices::storageLocation ( QDesktopServices::DataLocation );
  QDir d ( QDir::home() );
  if ( ! d.exists ( dest ) )
    d.mkpath ( dest );

  return QString ( "%1/bookmark.xml" ).arg ( dest );
}

/** \class BookmarkEntry
*/
BookmarkEntry::BookmarkEntry ( Bookmark *doc, const QString &id )
    : QDomElement ( doc->documentElement() )
{
  QDomElement rootNode = doc->createElement ( "entry" );
  rootNode.setAttribute ( "title", id );
  appendChild ( rootNode );

  vcodecNode = doc->createElement ( "vcodec" );
  rootNode.appendChild ( vcodecNode );

  acodecNode = doc->createElement ( "acodec" );
  rootNode.appendChild ( acodecNode );
}

BookmarkEntry::BookmarkEntry ( QDomElement &rootNode )
    : QDomElement ( rootNode )
{
  if ( firstChildElement ( "vcodec" ).nodeType() == QDomNode::ElementNode )
    vcodecNode = firstChildElement ( "vcodec" );
  else
  {
    vcodecNode = ownerDocument().createElement ( "vcodec" );
    rootNode.appendChild ( vcodecNode );
  }

  if ( firstChildElement ( "acodec" ).nodeType() == QDomNode::ElementNode )
    acodecNode = firstChildElement ( "acodec" );
  else
  {
    acodecNode = ownerDocument().createElement ( "acodec" );
    rootNode.appendChild ( acodecNode );
  }
}

const QString BookmarkEntry::getCodecName ( TYPE type )
{
  QString name;
  if ( type == VCODEC )
  {
    if ( vcodecNode.hasAttribute ( "id" ) )
      return vcodecNode.attribute ( "id" );
  }
  else if ( type == ACODEC )
  {
    if ( acodecNode.hasAttribute ( "id" ) )
      return acodecNode.attribute ( "id" );
  }
  return name;
}

void BookmarkEntry::setCodecOptions ( TYPE t, const QString &codecName, const QHash<QString,QVariant> &hash )
{
  QDomDocument doc = ownerDocument();
  QString id = codecName.trimmed();

  QString nodeName = ( t == VCODEC ) ? "vcodec" : "acodec";
  QDomElement sharedNode = doc.createElement ( nodeName );
  sharedNode.setAttribute ( "id", id );

  QHash<QString,QVariant> h = hash;
  QHash<QString,QVariant>::iterator it;
  for ( it = h.begin(); it != h.end(); ++it )
  {
    QDomElement e = doc.createElement ( "entry" );
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
    QDomNodeList nodes = codecNode.elementsByTagName ( "entry" );
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

/** \class Bookmark */
Bookmark::Bookmark ()
    : QDomDocument()
{
  appendChild ( createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"utf-8\" " ) );
  QDomElement rootNode = createElement ( "bookmark" );
  appendChild ( rootNode );
}

bool Bookmark::open ()
{
  QFile fp ( bookmarkDataPath() );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    QString errorMsg;
    int errorLine;
    int errorColumn;
    bool status = setContent ( &fp, &errorMsg, &errorLine, &errorColumn );
    if ( ! status )
      qWarning ( "Project Reader Error\n\t%s at %d %d", qPrintable ( errorMsg ), errorLine, errorColumn );

    fp.close();
    return status;
  }
  return false;
}

const QStringList Bookmark::entries()
{
  QStringList list;
  if ( hasChildNodes() )
  {
    QDomNodeList nodes = elementsByTagName ( "entry" );
    for ( int n = 0; n < nodes.size(); ++n )
    {
      QDomElement e = nodes.item ( n ).toElement();
      if ( e.hasAttribute ( "title" ) )
        list.append ( e.attribute ( "title", "" ) );
    }
  }
  return list;
}

const BookmarkEntry Bookmark::entry ( const QString &id )
{
  Q_ASSERT ( ! ( id.isEmpty() ) );
  QString ref = id.trimmed();
  if ( hasChildNodes() )
  {
    QDomNodeList nodes = elementsByTagName ( "entry" );
    for ( int n = 0; n < nodes.size(); ++n )
    {
      QDomElement e = nodes.item ( n ).toElement();
      if ( ! e.hasAttribute ( "title" ) )
        continue;

      if ( e.attribute ( "title" ).compare ( ref ) == 0 )
        return static_cast<BookmarkEntry> ( e );
    }
  }
  return BookmarkEntry ( this, ref );
}

bool Bookmark::removeEntryById ( const QString &id )
{
  if ( hasChildNodes() )
  {
    QString ref = id.trimmed();
    QDomNodeList nodes = elementsByTagName ( "entry" );
    for ( int n = 0; n < nodes.size(); ++n )
    {
      QDomElement e = nodes.item ( n ).toElement();
      if ( ! e.hasAttribute ( "title" ) )
        continue;

      if ( e.attribute ( "title" ).compare ( ref ) == 0 )
      {
        documentElement().removeChild ( e );
        break;
      }
    }
    return save();
  }
  return false;
}

bool Bookmark::save () const
{
  QFile fp ( bookmarkDataPath() );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << toByteArray ( 1 );
    fp.close();
    fp.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner ) );
    return true;
  }
  return false;
}

bool Bookmark::save ( QDomDocument * xml ) const
{
  QFile fp ( bookmarkDataPath() );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << xml->toByteArray ( 1 );
    fp.close();
    fp.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner ) );
    return true;
  }
  return false;
}

Bookmark::~Bookmark()
{}

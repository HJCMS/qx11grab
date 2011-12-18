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
#include <QtCore/QTextStream>

/* QtGui */
#include <QtGui/QDesktopServices>

/* QtXml */
#include <QtXml/QDomNode>
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

/** \class BookmarkEntry */
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

BookmarkEntry::BookmarkEntry ( const QDomElement &parent )
    : QDomElement ( parent )
{}

void BookmarkEntry::addVCodecs ( const QString &id, const QHash<QString,QVariant> &hash )
{
  vcodecNode.setAttribute ( "id", id );
  QDomDocument doc = ownerDocument();
  QHash<QString,QVariant> h = hash;
  QHash<QString,QVariant>::iterator it;
  for ( it = h.begin(); it != h.end(); ++it )
  {
    QDomElement e = doc.createElement ( "entry" );
    vcodecNode.appendChild ( e );
    QDomElement a = doc.createElement ( "argument" );
    a.appendChild ( doc.createCDATASection ( it.key() ) );
    e.appendChild ( a );
    QDomElement v = doc.createElement ( "value" );
    v.appendChild ( doc.createCDATASection ( it.value().toString() ) );
    e.appendChild ( v );
  }
}

void BookmarkEntry::addACodecs ( const QString &id, const QHash<QString,QVariant> &hash )
{
  acodecNode.setAttribute ( "id", id );
  QDomDocument doc = ownerDocument();
  QHash<QString,QVariant> h = hash;
  QHash<QString,QVariant>::iterator it;
  for ( it = h.begin(); it != h.end(); ++it )
  {
    QDomElement e = doc.createElement ( "entry" );
    acodecNode.appendChild ( e );
    QDomElement a = doc.createElement ( "argument" );
    a.appendChild ( doc.createCDATASection ( it.key() ) );
    e.appendChild ( a );
    QDomElement v = doc.createElement ( "value" );
    v.appendChild ( doc.createCDATASection ( it.value().toString() ) );
    e.appendChild ( v );
  }
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
  QDomNodeList nodes = elementsByTagName ( "entry" );
  for ( int n = 0; n < nodes.size(); ++n )
  {
    QDomElement e = nodes.item ( n ).toElement();
    if ( e.hasAttribute ( "title" ) )
      list.append ( e.attribute ( "title", "" ) );
  }
  return list;
}

const BookmarkEntry Bookmark::entry ( const QString &id )
{
  Q_ASSERT ( ! ( id.isEmpty() ) );

  QDomNodeList nodes = elementsByTagName ( "entry" );
  for ( int n = 0; n < nodes.size(); ++n )
  {
    QDomElement e = nodes.item ( n ).toElement();
    if ( e.attribute ( "title", "NULL" ) == id )
      return static_cast<BookmarkEntry> ( e );
  }
  return BookmarkEntry ( this, id );
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

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
  if ( documentElement().hasChildNodes() )
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

BookmarkEntry Bookmark::entry ( const QString &id )
{
  Q_ASSERT ( ! ( id.isEmpty() ) );
  QString ref = id.trimmed();
  if ( documentElement().hasChildNodes() )
  {
    QDomNodeList nodes = elementsByTagName ( "entry" );
    for ( int n = 0; n < nodes.size(); ++n )
    {
      QDomElement e = nodes.item ( n ).toElement();
      if ( ! e.hasAttribute ( "title" ) )
        continue;

      if ( e.attribute ( "title" ).compare ( ref ) == 0 )
        return BookmarkEntry ( e );
    }
  }

  QDomElement rootNode = createElement ( "entry" );
  rootNode.setAttribute ( "title", ref );
  documentElement().appendChild ( rootNode );
  BookmarkEntry entry ( rootNode );
  entry.initDefaults();
  return entry;
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

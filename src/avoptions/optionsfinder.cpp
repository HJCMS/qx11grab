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
*/

#include "optionsfinder.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

/* QtXml */
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

namespace QX11Grab
{
  /**
  * Diese Klasse sucht die Options XML Dateien und liest bei einem
  * Treffer nach den passenden Vorgabe Optionen.
  * \param codec Der Aktuell ausgewählte Kodierer Name
  */
  OptionsFinder::OptionsFinder ( const QString &codec )
      : QDomDocument ( "encoder" )
      , p_codec ( codec )
      , p_template ( QString::null )
      , p_isOpen ( false )
  {
    if ( ! p_codec.isEmpty() )
      initTemplate();
  }

  /**
  * Suche ausgehend vom Binär Programm nach dem Template Pfad
  * und der mit \ref p_codec gsetzten Kodierere Datei.
  * \li Suche nach ?/share/qx11grab/options/codec.xml,
  * \li Wenn gefunden Lese XML Datei in \ref p_template ein,
  * \li Führe \e QDomDocument::setContent aus.
  */
  void OptionsFinder::initTemplate()
  {
    QString path;
#ifdef MAINTAINER_REPOSITORY
    QString projectHome ( qgetenv("HJCMS_PROJECTS_DIR") );
    path = QString::fromUtf8 ( "%1/%2/src/avoptions/options" ).arg ( projectHome, qApp->applicationName() );
#else
    path = QString::fromUtf8 ( "%1/../share/%2/options" ).arg ( qApp->applicationDirPath(), qApp->applicationName() );
#endif
    QDir d ( path );
    foreach ( QString xml, d.entryList ( QStringList ( "*.xml" ), ( QDir::Files | QDir::NoSymLinks ) ) )
    {
      if ( xml.contains ( p_codec + ".xml" ) )
      {
        p_template = QString::fromUtf8 ( "%1/%2" ).arg ( d.absolutePath(), xml );
        break;
      }
    }

    if ( p_template.isNull() )
      return;

    QFile fp ( p_template );
    if ( fp.open ( QIODevice::ReadOnly ) )
    {
      if ( setContent ( &fp ) )
        p_isOpen = true;

      fp.close();
    }

  }

  /**
  * Lese alle Prädikate \b param von den \b options Elementen
  * aus und gebe Sie in einer Liste zurück.
  */
  const QStringList OptionsFinder::options()
  {
    QStringList opts;
    if ( p_template.isNull() )
      return opts;

    if ( p_isOpen )
    {
      QDomNodeList nodes = elementsByTagName ( "option" );
      for ( int n = 0; n < nodes.size(); ++n )
      {
        QDomElement e = nodes.item ( n ).toElement();
        if ( e.hasAttributes() )
          opts.append ( e.attribute ( "param" ) );
      }
    }

    return opts;
  }

  /**
  * Wenn das angefragte Options Element zusätzliche Vorgabe
  * Knoten besitzt gebe dies in einer Liste zurück.
  * \param option Zeiger auf das Prädikat \b param des option Elementes
  */
  const QStringList OptionsFinder::values ( const QString &option )
  {
    if ( ! p_isOpen )
      return QStringList();

    QStringList buffer;
    QString argument ( option );
    argument.remove ( QRegExp ( "^[\\-]+" ) );
    if ( ! argument.isEmpty() )
    {
      QDomNodeList nodes = elementsByTagName ( "option" );
      for ( int n = 0; n < nodes.size(); ++n )
      {
        if ( ! nodes.item ( n ).hasChildNodes() )
          continue;

        QDomElement e = nodes.item ( n ).toElement();
        if ( e.hasAttributes() && ( e.attribute ( "param", "NULL" ) == argument ) )
        {
          QDomNodeList childs = e.elementsByTagName ( "value" );
          for ( int j = 0; j < childs.size(); ++j )
          {
            buffer.append ( childs.item ( j ).firstChild().nodeValue() );
          }
        }
      }
    }
    return buffer;
  }

  /**
  * Erstellt eine Liste der Verfügbaren Erweiterungen aus den \b ext Elementen.
  * \note Jedes Template hat Mindestens ein ext Element!
  */
  const QList<VideoExtension> OptionsFinder::extensionList()
  {
    QList<VideoExtension> list;

    if ( p_isOpen )
    {
      QDomNodeList nodes = elementsByTagName ( "ext" );
      for ( int n = 0; n < nodes.size(); ++n )
      {
        QDomElement e = nodes.item ( n ).toElement();
        if ( e.hasAttributes() )
        {
          VideoExtension vext;
          vext.name = e.attribute ( "name" );
          vext.description = e.firstChild().nodeValue();
          vext.isDefault = e.hasAttribute ( "default" );
          list.append ( vext );
        }
      }
    }

    return list;
  }

  OptionsFinder::~OptionsFinder()
  {}
}  /* eof namespace QX11Grab */

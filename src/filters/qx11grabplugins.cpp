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

#include "qx11grabplugins.h"
#include "interface.h"

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QPluginLoader>
#include <QtCore/QLibrary>
#include <QtCore/QLibraryInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QSysInfo>

namespace QX11Grab
{
  Plugins::Plugins ( QObject * parent )
      : QObject ( parent )
      , p_dir ( qApp->applicationDirPath() )
  {
    setObjectName ( QLatin1String ( "Plugins" ) );

    QStringList paths; // Alle Suchverzeichnisse
    paths << QString ( "%1/plugins" ).arg ( p_dir.absolutePath() ); // NOTE ganz vorne
    paths << QString ( "%1/qx11grab" ).arg ( QLibraryInfo::location ( QLibraryInfo::LibrariesPath ) );
    paths << QString ( QX11GRAB_MODULES_PATH );
    if ( QSysInfo::WordSize == 64 )
      paths << QString ( "%1/../lib64/qx11grab" ).arg ( p_dir.absolutePath() );
    else
      paths << QString ( "%1/../lib/qx11grab" ).arg ( p_dir.absolutePath() );

    p_dir.setSearchPaths ( "plugins", paths );
  }

  /**
  * Die Plugin Verzeichnisse durchgehen und nachsehen ob Plugins vorhanden sind.
  */
  const QStringList Plugins::files ()
  {
    /* Verhindern das gleiche Plugins aus mehreren
    * Verzeichnissen dargestellt werden! */
    QStringList unique;
    QStringList plugins; // Liste der Plugins

    foreach ( QString path, p_dir.searchPaths ( "plugins" ) )
    {
      p_dir.setPath ( path );
      foreach ( QString p, p_dir.entryList ( QStringList ( "*.so" ), QDir::Files, QDir::Name ) )
      {
        QFileInfo info ( p );
        if ( QLibrary::isLibrary ( p ) && ! unique.contains ( info.baseName() ) )
        {
          // qDebug() << Q_FUNC_INFO << "Found:" << path << info.baseName();
          unique << info.baseName();
          plugins << QString ( "%1%2%3" ).arg ( p_dir.path(), p_dir.separator(), p );
        }
      }
    }
    unique.clear();
    return plugins;
  }

  /** Suche mit Name aus @ref PlugInfo nach dem Pluginpfad
  * Wenn nicht gefunden wurde ist die Zeichenkette \b null
  */
  bool Plugins::find ( const QString &libName )
  {
    foreach ( QString fp, files() )
    {
      if ( fp.contains ( libName, Qt::CaseInsensitive ) )
        return true;
    }
    return false;
  }

  QX11Grab::Interface* Plugins::get ( const QString &name, QWidget * parent )
  {
    Q_ASSERT ( parent );
    QX11Grab::Interface* m_interface = 0;
    foreach ( QString fp, files() )
    {
      if ( fp.contains ( name, Qt::CaseInsensitive ) )
      {
        QPluginLoader loader ( fp, this );
        QObject* plug = loader.instance();
        if ( plug )
        {
          m_interface = static_cast<QX11Grab::Interface*> ( plug );
          if ( m_interface && m_interface->create ( parent ) )
            return m_interface;
        }
      }
    }
    return m_interface;
  }

  /** Lese alle Plugins in eine Liste */
  const QList<QX11Grab::Interface*> Plugins::getAll ( QWidget * parent )
  {
    Q_ASSERT ( parent );
    QList<QX11Grab::Interface*> list;
    foreach ( QString fp, files() )
    {
      QPluginLoader loader ( fp, this );
      QObject* plug = loader.instance();
      if ( plug )
      {
        QX11Grab::Interface* m_interface = static_cast<QX11Grab::Interface*> ( plug );
        if ( m_interface && m_interface->create ( parent ) )
          list << m_interface;
      }
      else
        qWarning ( "(QX11Grab) Plugins: %s", qPrintable ( loader.errorString() ) );
    }
    return list;
  }

  Plugins::~Plugins()
  {}

} /* eof namespace qx11grab */

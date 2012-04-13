/*
* This file is part of the qx11grab/xhtmldbg project
*
* Copyright (C) Juergen Heinemann (Undefined) http://www.hjcms.de, (C) 2007-2012
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

#include "iconthemeselector.h"
#include "iconthememodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

IconThemeSelector::IconThemeSelector ( QWidget * parent )
    : QComboBox ( parent )
    , m_model ( 0 )
{
  setObjectName ( QLatin1String ( "IconTheme" ) );
  m_model = new IconThemeModel ( this );
  setModel ( m_model );

  QStringList list ( "/usr/share/icons" );
  list << "/usr/share/kde/icons";
  list << "/usr/share/kde4/icons";
  list << "/usr/local/share/icons";
  list << "/opt/kde4/share/icons";
  findThemeIndexes ( list );

  connect ( this, SIGNAL ( currentIndexChanged ( const QString & ) ),
            this, SLOT ( indexChanged ( const QString & ) ) );
}

/**
* Wenn eine Auswahl getroffen wurde das Signal
* @ref modified abstoßen.
*/
void IconThemeSelector::indexChanged ( const QString &txt )
{
  Q_UNUSED ( txt )
  emit modified ( true );
}

/**
* Alle Inhalte aus dem Model entfernen!
*/
void IconThemeSelector::clearContent ()
{
  m_model->clear();
}

/**
* Suche in @ref QIcon::themeSearchPaths nach den Themes
*/
void IconThemeSelector::findThemeIndexes ( const QStringList &list )
{
  QDir dir;
  QStringList filter ( "*" );
  QStringList themeDirs = list;
  themeDirs.removeDuplicates();

  // erst Bereinigen
  clearContent();

  foreach ( QString p, themeDirs )
  {
    dir.setPath ( p );
    if ( dir.canonicalPath() != dir.absolutePath() )
      continue; // symbolischen verknüpfungen nicht folgen!

    if ( dir.dirName().contains ( QRegExp ( "\\bdefault\\b", Qt::CaseInsensitive ) ) )
      continue;

    if ( dir.exists() )
    {
      qDebug() << Q_FUNC_INFO << dir.absolutePath();
      foreach ( QString f, dir.entryList ( filter, ( QDir::Dirs | QDir::NoSymLinks ), QDir::Name ) )
      {
        QFileInfo info ( dir, QString ( "%1/index.theme" ).arg ( f ) );
        if ( info.exists() )
          m_model->insert ( info.absoluteFilePath() );
      }
    }
  }
}

/**
* Setz den Verzeichniss Namen für das aktuelle Icon Thema
*/
void IconThemeSelector::setValue ( const QString &theme )
{
  int index = findData ( theme, Qt::UserRole );
  if ( index > 0 )
    setCurrentIndex ( index );
  else
    setCurrentIndex ( 0 );
}

/**
* Gibt den Verzeichniss namen des ausfewählten Themas zurück!
*/
const QString IconThemeSelector::value()
{
  int index = currentIndex();
  return itemData ( index, Qt::UserRole ).toString();
}

IconThemeSelector::~IconThemeSelector()
{}

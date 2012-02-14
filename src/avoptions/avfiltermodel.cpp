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

#include "avfiltermodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringMatcher>

namespace QX11Grab
{
  /**
  * \class AVFilterItem
  */
  AVFilterItem::AVFilterItem ( const QString &f )
      : name ( QString::null )
      , filter ( f.trimmed() )
      , hasInLabel ( false )
      , hasPipeLabel ( false )
  {
    /**
    * Suche den Filter Parameter und schreibe ihn zum Sortieren
    * nach Name. NOTE Labels müssen hier entfernt werden!
    */
    QString buffer = filter.left ( filter.indexOf ( '=', 0 ) );
    if ( buffer.contains ( "[" ) )
      name = buffer.remove ( QRegExp ( "^(\\[[\\w\\d]+\\])+" ) ).trimmed();
    else
      name = buffer.trimmed();

    /**
    * Völlig egal ob ein [out] Label existiert, es kommt ohnehin an
    * das Ende der Definition! Wann ein [out] angehangen wird,
    * wird von \ref hasPipeLabel bestimmt. Also weg damit!
    */
    filter.remove ( QRegExp ( "\\[out\\]" ) );

    /**
    * Ein Input Label kann weitere Labels besitzen!
    * Muss aber hinter dem 1. Referenzierten Anker Label stehen!
    * Deshalb hier einen Marker setzen!
    */
    QRegExp inputPattern ( "\\[in\\]" );
    hasInLabel = filter.contains ( inputPattern );

    /**
    * Suche nach vorhandenen Ankern und setze bei einem Treffer
    * \ref hasPipeLabel auf true. Gleichzeitig werden die Start-
    * Labels für die Sortierung extrahiert und in \ref labels geschrieben!
    */
    QRegExp labelPattern ( "(\\[[\\d\\w]+\\][\\s\\t]*)$" );
    if ( filter.contains ( labelPattern ) )
    {
      QStringList l = labelPattern.capturedTexts();
      l.removeDuplicates();
      labels = l.join ( "" );
      hasPipeLabel = true;
      l.clear();
      // qDebug() << Q_FUNC_INFO << labels;
    }
  }

  /**
  * \class AVFilterModel
  */
  AVFilterModel::AVFilterModel ( const QString &filter )
      : currentFilters ( filter.split ( QX11GRAB_FILTER_DELIMITER ) )
      , isPiped ( false )
  {
    /* Filter in Liste setzen */
    filters.clear();
    foreach ( QString item, currentFilters )
    {
      AVFilterItem it ( item );
      isPiped = ( ( isPiped ) ? isPiped : ( it.hasInLabel || it.hasPipeLabel ) );
      filters.append ( it );
    }
  }

  /**
  * Er/Setzt Filter in der Liste
  */
  void AVFilterModel::insertFilter ( const QVariant &data )
  {
    // Temporäre Filterliste
    QStringList buffer;

    // Neuer Filter
    QString filter = data.toString();

    // Den Filter Parameter vom übergebenen Inhalt extrahieren
    QString parameter = filter.left ( filter.indexOf ( '=', 0 ) );
    QRegExp pattern ( "(^"+parameter+"=)" );

    /* Filter in Liste setzen */
    foreach ( QString item, currentFilters )
    {
      // Soll der Filter überschrieben werden?
      if ( ! item.contains ( pattern ) )
        buffer.append ( item );
    }
    buffer.append ( filter );

    // Wenn der Puffer nicht leer ist dann die Filterliste neu erstellen.
    if ( ! buffer.isEmpty() )
    {
      // Primäre Filterliste neu Setzen
      currentFilters.clear();
      currentFilters.append ( buffer );

      // Referenzierte Filterliste neu Setzen
      filters.clear();
      foreach ( QString f,  buffer )
      {
        AVFilterItem it ( f );
        isPiped = ( ( isPiped ) ? isPiped : ( it.hasInLabel || it.hasPipeLabel ) );
        filters.append ( it );
      }
    }

    // puffer freigeben
    buffer.clear();
  }

  /**
  * Sortierte Filterliste zurück geben!
  */
  const QString AVFilterModel::values()
  {
    // Sind labels vorhanden dann sortieren!
    if ( isPiped )
    {
      QString label; // Steht ganz vorne!
      QString input; // Steht an zweiter stelle!
      QStringList other; // werden angehangen
      QList<AVFilterItem>::iterator it;
      for ( it = filters.begin(); it != filters.end(); ++it )
      {
        // Eingabe Filter mit Ausgabe Label <param>=...[??],
        if ( it->hasPipeLabel )
        {
          label.append ( it->filter );
          label.append ( "," );
        }
        else if ( it->hasInLabel )
        {
          label.append ( it->filter );
          label.append ( "," );
        }
        else
          other.append ( it->filter );
      }
      QString data;
      data.append ( label );
      data.append ( input );
      data.append ( other.join ( QX11GRAB_FILTER_DELIMITER ) );
      data.append ( "[out]" );

#ifdef MAINTAINER_REPOSITORY
      qDebug() << Q_FUNC_INFO << data;
#endif

      return data;
    }
    return currentFilters.join ( QX11GRAB_FILTER_DELIMITER );
  }

  AVFilterModel::~AVFilterModel()
  {
    currentFilters.clear();
    filters.clear();
  }

}  /* eof namespace QX11Grab */

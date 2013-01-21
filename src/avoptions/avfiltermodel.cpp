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

#include "avfiltermodel.h"
#include "avfilteritem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringMatcher>

namespace QX11Grab
{

  AVFilterModel::AVFilterModel ( const QString &filter )
      : currentFilters ( filter.split ( QX11GRAB_FILTER_DELIMITER ) )
      , isPiped ( false )
  {
    /* Filter in Liste setzen */
    filters.clear();
    foreach ( QString item, currentFilters )
    {
      AVFilterItem* it = new AVFilterItem ( item );
      isPiped = ( ( isPiped ) ? isPiped : ( it->hasInLabel() || it->hasPipeLabel() ) );
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

    /* Schreibe die Aktuelle Filterliste in den Puffer
    * und grenze die Einträge aus die mit dem gleichen
    * Parameter und/oder Label beginnen!
    */
    foreach ( QString item, currentFilters )
    {
      QString param = item.left ( item.indexOf ( '=', 0 ) );
      param.append ( "=" );
      if ( ! filter.contains ( param, Qt::CaseSensitive ) )
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
        AVFilterItem* it = new AVFilterItem ( f );
        isPiped = ( it->hasInLabel() || it->hasPipeLabel() );
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
      for ( int i = 0; i < filters.size(); ++i )
      {
        AVFilterItem* item = filters.at ( i );
        // qDebug() << __LINE__ << item->filterName() << item->hasPipeLabel() << item->hasInLabel() << item->filter();
        if ( item->hasPipeLabel() )
        {
          // Eingabe Filter mit Ausgabe Label <param>=...[??],
          label.prepend ( "," );
          label.prepend ( item->filter() );
        }
        else if ( item->hasInLabel() )
        {
          // Ausgangs Filter mit [in][??]<param>=...,
          input.append ( item->filter() );
          input.append ( "," );
        }
        else
        {
          // Standard Filter <param>=...,
          other.append ( item->filter() );
        }

      }

      QString data;
      data.append ( label );
      data.append ( input );
      data.append ( other.join ( QX11GRAB_FILTER_DELIMITER ) );
      data.append ( "[out]" );

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

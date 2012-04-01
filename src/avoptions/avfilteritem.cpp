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

#include "avfilteritem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QStringMatcher>

namespace QX11Grab
{
  static const QRegExp isPipedPattern()
  {
    return QRegExp ( "(\\[[\\d\\w]+\\][\\s\\t]*)[$\\,]{1}" );
  }

  AVFilterItem::AVFilterItem ( const QString &data )
      : p_labels ( QString() )
      , p_filter ( QString() )
  {
    /**
    * Völlig egal ob ein [out] Label existiert, es kommt ohne hin darauf
    * an das es am Ende der Definition steht!
    * Wann ein [out] angehangen wird, wird von \ref hasPipeLabel bestimmt.
    * Also weg damit!
    */
    QString buf = data.trimmed();
    QRegExp pattern ( "\\[out\\]", Qt::CaseInsensitive );
    p_filter = buf.remove ( pattern );
    buf.clear();
  }

  /**
  * Extrahiere den Filter Parameter
  * NOTE Labels müssen hier entfernt werden!
  */
  const QString AVFilterItem::filterName()
  {
    QString buffer = p_filter.left ( p_filter.indexOf ( '=', 0 ) );
    if ( buffer.contains ( "[" ) )
      return buffer.remove ( QRegExp ( "^(\\[[\\w\\d]+\\])+" ) ).trimmed();
    else
      return buffer.trimmed();
  }

  /**
  * Ein Input Label kann weitere Labels besitzen!
  * Muss aber hinter dem 1. Referenzierten Anker Label stehen!
  * Deshalb hier einen Marker setzen!
  */
  bool AVFilterItem::hasInLabel()
  {
    QRegExp pattern ( "\\[in\\]", Qt::CaseInsensitive );
    return p_filter.contains ( pattern );
  }

  /**
  * Suche nach vorhandenen Ankern und setze bei einem Treffer
  * \ref hasPipeLabel auf true. Gleichzeitig werden die Start-
  * Labels für die Sortierung extrahiert und in \ref labels geschrieben!
  */
  bool AVFilterItem::hasPipeLabel()
  {
    bool b = false;
    QRegExp pattern = isPipedPattern();
    if ( p_filter.contains ( pattern ) )
    {
      QStringList l = pattern.capturedTexts();
      l.removeDuplicates();
      p_labels = l.join ( "" );
      b = true;
      l.clear();
    }
    return b;
  }

  const QString AVFilterItem::filter()
  {
    return p_filter;
  }

  const QString AVFilterItem::labels()
  {
    return p_labels;
  }

  bool AVFilterItem::isPipeFilter ( const QString &filter )
  {
    return filter.contains ( isPipedPattern() );
  }

  AVFilterItem::~AVFilterItem()
  {
    p_filter.clear();
    p_labels.clear();
  }

};  /* eof namespace QX11Grab */


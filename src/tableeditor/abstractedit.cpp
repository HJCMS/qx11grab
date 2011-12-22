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

#include "abstractedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QCompleter>

AbstractEdit::AbstractEdit ( QWidget * parent )
    : QLineEdit ( parent )
    , m_regExpValidator ( new QRegExpValidator ( this ) )
{
  QRegExp pattern ( "[^ \n\t&\\]+" );
  m_regExpValidator->setRegExp ( pattern );
}

const QString AbstractEdit::implode ( const QStringList &data ) const
{
  QString buffer ( data.join ( "," ) );
  return buffer.trimmed();
}

const QStringList AbstractEdit::explode ( const QString &data ) const
{
  QString buffer ( data );
  return buffer.split ( "," );
}

void AbstractEdit::setValue ( const QVariant &value )
{
  setText ( value.toString() );
}

void AbstractEdit::setCompleters ( const QStringList &list )
{
  if ( list.size() > 0 )
  {
    QCompleter* m_compliter = new QCompleter ( list, this );
    setCompleter ( m_compliter );
  }
}

const QVariant AbstractEdit::value()
{
  return QVariant ( text() );
}

const QByteArray AbstractEdit::valuePropertyName () const
{
  return QByteArray ( "value" );
}

AbstractEdit::~AbstractEdit()
{}

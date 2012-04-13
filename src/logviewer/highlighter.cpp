/*
* This file is part of the xhtmldbg project
*
* Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2012
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

#include "highlighter.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QPalette>

Highlighter::Highlighter ( QTextDocument *parent )
    : QSyntaxHighlighter ( parent )
    , block_stat ( 0 )
{
  QPalette pl = qApp->palette();
  QBrush bg = pl.brush ( QPalette::NoRole );

  RuleHighlight step1; // Strings
  step1.pattern = QRegExp ( "(?!=[\\s\\>\\/])([\\\"\\\'](.*)[\\\"\\\'])" );
  stringFormat.setForeground ( Qt::red );
  step1.format = stringFormat;
  highlightRules.append ( step1 );

  RuleHighlight step2; // Predicates
  step2.pattern = QRegExp ( "([^|\\s][\\w]+(?=[=]))" );
  attributeFormat.setForeground ( Qt::blue );
  step2.format = attributeFormat;
  highlightRules.append ( step2 );

  RuleHighlight step3; // KeyWords
  step3.pattern = QRegExp ( "((^|\\s+)[a-zA-Z][\\w]+:)" );
  keywordFormat.setForeground ( Qt::blue );
  step3.format = keywordFormat;
  highlightRules.append ( step3 );

  RuleHighlight step4; // Error
  step4.pattern = QRegExp ( "\\b(error|fatal|warning|invalid)", Qt::CaseInsensitive );
  errorFormat.setForeground ( Qt::darkGray );
  step4.format = errorFormat;
  highlightRules.append ( step4 );
}

void Highlighter::highlightBlock ( const QString &text )
{
  if ( text.isEmpty() )
    return;

  setCurrentBlockState ( ++block_stat );

  foreach ( const RuleHighlight &r, highlightRules )
  {
    QRegExp reg ( r.pattern );
    int in = text.indexOf ( reg );
    while ( in >= 0 )
    {
      int le = reg.matchedLength();
      setFormat ( in, le, r.format );
      in = text.indexOf ( reg, in + le );
    }
  }
}

Highlighter::~Highlighter()
{
  highlightRules.clear();
}

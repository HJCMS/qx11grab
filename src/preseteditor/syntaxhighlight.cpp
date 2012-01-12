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

#include "syntaxhighlight.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QTextDocument>
#include <QtGui/QFont>
#include <QtGui/QPalette>

SyntaxHighlight::SyntaxHighlight ( QTextEdit * parent )
    : QSyntaxHighlighter ( parent->document() )
    , block_stat ( 0 )
{
  setObjectName ( QLatin1String ( "SyntaxHighlight" ) );
  QPalette pl = qApp->palette();
  QBrush bg = pl.brush ( QPalette::NoRole );

  keywordFormat.setForeground ( Qt::darkGreen );
  numericFormat.setForeground ( Qt::darkRed );
  commentFormat.setForeground ( Qt::gray );

  QFont font = parent->currentFont();
  font.setBold ( true );
  operatorFormat.setForeground ( Qt::darkBlue );
  operatorFormat.setFont ( font );


  RuleHighlight step1;  // Predicates
  step1.pattern = QRegExp ( "(^[\\w_]+(?=[=]))" );
  step1.format = keywordFormat;
  highlightRules.append ( step1 );

  RuleHighlight step2;  // Numeric
  step2.pattern = QRegExp ( "((?!=[=])[\\d\\.]+$)" );
  step2.format = numericFormat;
  highlightRules.append ( step2 );

  RuleHighlight step3;  // Comments
  step3.pattern = QRegExp ( "([=\\+\\-])" );
  step3.format = operatorFormat;
  highlightRules.append ( step3 );

  RuleHighlight step4;  // Comments
  step4.pattern = QRegExp ( "(^#+.+$)" );
  step4.format = commentFormat;
  highlightRules.append ( step4 );

}

void SyntaxHighlight::highlightBlock ( const QString &text )
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

SyntaxHighlight::~SyntaxHighlight()
{
  highlightRules.clear();
}

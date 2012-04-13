/*
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
*/

#include "syntaxhighlight.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QTextDocument>
#include <QtGui/QFont>
#include <QtGui/QPalette>

/**
* @class SyntaxHighlight
* Syntax Hervorhebung für *.ffpreset Dokumente
*/
SyntaxHighlight::SyntaxHighlight ( QPlainTextEdit * parent )
    : QSyntaxHighlighter ( parent->document() )
    , block_stat ( 0 )
    , editor ( parent )
{
  setObjectName ( QLatin1String ( "SyntaxHighlight" ) );
  QPalette pl = qApp->palette();
  QBrush bg = pl.brush ( QPalette::NoRole );

  keywordFormat.setForeground ( Qt::darkGreen );
  numericFormat.setForeground ( Qt::darkRed );
  commentFormat.setForeground ( Qt::gray );

//   QFont font = editor->currentFont();
//   font.setBold ( true );
//   operatorFormat.setForeground ( Qt::darkBlue );
//   operatorFormat.setFont ( font );

  RuleHighlight step1;  // Prädikate
  step1.pattern = QRegExp ( "(^[\\w_]+(?=[=]))" );
  step1.format = keywordFormat;
  highlightRules.append ( step1 );

  RuleHighlight step2;  // Zahlen
  step2.pattern = QRegExp ( "((?!=[=])[\\d\\.]+$)" );
  step2.format = numericFormat;
  highlightRules.append ( step2 );

  RuleHighlight step3;  // Operatoren
  step3.pattern = QRegExp ( "([=\\+\\-])" );
  step3.format = operatorFormat;
  highlightRules.append ( step3 );

  RuleHighlight step4;  // Kommentare
  step4.pattern = QRegExp ( "(^#+.+$)" );
  step4.format = commentFormat;
  highlightRules.append ( step4 );

}

/**
* Textbausteine für die Hervorhebung durchlaufen
*/
void SyntaxHighlight::highlightBlock ( const QString &text )
{
  if ( text.isEmpty() )
    return;

  // setzt die Aktuelle Zeilennummer
  setCurrentBlockState ( ++block_stat );

  // vector durchlaufen
  foreach ( const RuleHighlight &r, highlightRules )
  {
    QRegExp expr ( r.pattern );
    int in = text.indexOf ( expr );
    while ( in >= 0 )
    {
      int le = expr.matchedLength();
      setFormat ( in, le, r.format );
      in = text.indexOf ( expr, in + le );
    }
  }
}

/**
* Beim beenden den Vector leeren
*/
SyntaxHighlight::~SyntaxHighlight()
{
  highlightRules.clear();
}

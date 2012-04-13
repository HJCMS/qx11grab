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

#ifndef SYNTAXHIGHLIGHT_H
#define SYNTAXHIGHLIGHT_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QVector>

/* QtGui */
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextBlock>

class SyntaxHighlight : public QSyntaxHighlighter
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    int block_stat;
    const QPlainTextEdit* editor;
    struct RuleHighlight
    {
      QRegExp pattern;
      QTextCharFormat format;
    };
    QVector<RuleHighlight> highlightRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat numericFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat commentFormat;

  protected:
    void highlightBlock ( const QString &text );

  public:
    /**
    * \class SyntaxHighlight
    * Syntax Highlighter for *.ffpreset Documents
    */
    explicit SyntaxHighlight ( QPlainTextEdit * parent );

    virtual ~SyntaxHighlight();
};

#endif

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

#include "presetedit.h"
#include "syntaxhighlight.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

PresetEdit::PresetEdit ( QWidget * parent )
    : QTextEdit ( parent )
    , docPath ( QString::null )
{
  setObjectName ( QLatin1String ( "PresetEdit" ) );
  setReadOnly ( false );
  setAcceptRichText ( false );
  setAutoFormatting ( QTextEdit::AutoNone );
  setWordWrapMode ( QTextOption::WordWrap );
  setLineWrapMode ( QTextEdit::WidgetWidth );
  setTextInteractionFlags ( Qt::TextEditorInteraction );
  setAlignment ( ( Qt::AlignLeft | Qt::AlignTop ) );
  new SyntaxHighlight ( this );
}

void PresetEdit::openPreset ( const QString &path )
{
  QFile fp ( path );
  if ( fp.open ( QIODevice::ReadOnly ) )
  {
    setDocumentPath ( path );
    QTextStream stream ( &fp );
    setPlainText ( stream.readAll() );
    fp.close();
  }
}

void PresetEdit::setDocumentPath ( const QString &path )
{
  docPath = path;
}

const QString PresetEdit::documentPath()
{
  return docPath;
}

const QPair<QString,QString> PresetEdit::data()
{
  QPair<QString,QString> p;
  if ( ( ! documentPath().isEmpty() ) && ( ! toPlainText().isEmpty() ) )
  {
    p.first = documentPath();
    p.second = toPlainText();
  }
  return p;
}

PresetEdit::~PresetEdit()
{}

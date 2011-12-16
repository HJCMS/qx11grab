/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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

#ifndef QX11GRAB_VERSION
# include "version.h"
#endif

#include "textview.h"
#include "highlighter.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>
#include <QtGui/QTextOption>
#include <QtGui/QMenu>
#include <QtGui/QPalette>

TextView::TextView ( QWidget * parent )
    : QTextEdit ( parent )
    , m_highlighter ( 0 )
    , source ( trUtf8 ( "wait for input ..." ) )
{
  setObjectName ( "TextView" );
  setContentsMargins ( 0, 0, 0, 0 );
  setMinimumWidth ( 100 );
  setMinimumHeight ( 100 );
  setAcceptRichText ( false );
  setAutoFormatting ( QTextEdit::AutoNone );
  setWordWrapMode ( QTextOption::WordWrap );
  setLineWrapMode ( QTextEdit::WidgetWidth );
  setUndoRedoEnabled ( false );
  setTextInteractionFlags ( ( Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard ) );
  setPlainText ( source );

  m_highlighter = new Highlighter ( document() );
}

/**
* Kontext Menü für das Editieren
*/
void TextView::contextMenuEvent ( QContextMenuEvent * e )
{
  QMenu* m = new QMenu ( this );

  QAction* ac_reload = m->addAction ( getThemeIcon ( "view-refresh" ), trUtf8 ( "Refresh" ) );
  ac_reload->setToolTip ( trUtf8 ( "Refresh Logfile" ) );
  ac_reload->setStatusTip ( trUtf8 ( "Refresh Logfile" ) );
  connect ( ac_reload, SIGNAL ( triggered() ), this, SIGNAL ( refresh() ) );

  m->exec ( e->globalPos() );
}

void TextView::gotoLine ( int r )
{
  QTextBlock tb = document()->findBlockByLineNumber ( r );
  if ( tb.isValid() )
  {
    blockSignals ( true );
    QString txt = tb.text();
    scrollToAnchor ( txt );
    setVisible ( true );

    setFocus ( Qt::ActiveWindowFocusReason );

    QTextCursor cur = textCursor();
    cur.setPosition ( tb.position() );
    setTextCursor ( cur );
    blockSignals ( false );
  }
}

void TextView::insertText ( const QString &data )
{
  if ( data.isEmpty() )
    return;

  bool b = false;
  QStringList buffer;
  foreach ( QString l, data.split ( QRegExp ( "[\\r\\n]" ) ) )
  {
    if ( ( b == false ) && ( l.contains ( QRegExp ( "\\[x11grab[\\s]+@" ) ) ) )
      b = true;

    if ( b )
      buffer.append ( l );
  }

  clear();
  setPlainText ( buffer.join ( "\n" ) );
  gotoLine ( ( document()->lineCount() - 1 ) );
}

TextView::~TextView()
{}

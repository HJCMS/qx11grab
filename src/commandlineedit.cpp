/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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

#include "commandlineedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QAbstractItemView>
#include <QtGui/QClipboard>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>
#include <QtGui/QPalette>

CommandLineEdit::CommandLineEdit ( QWidget * parent )
    : QListWidget ( parent )
{
  setObjectName ( "commandlineedit" );
  setSortingEnabled ( false );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setBackgroundRole ( QPalette::AlternateBase );
  setDragDropMode ( QAbstractItemView::InternalMove );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setDragDropOverwriteMode ( false );
  setAlternatingRowColors ( true );
}

/**
* Kopiere den aktuellen Listen Inhalt ins das Clipboard
*/
void CommandLineEdit::clipper()
{
  QApplication::clipboard()->setText ( data().join ( " " ) );
}

/**
* Sende das Signal @ref dataSaved
*/
void CommandLineEdit::save()
{
  emit dataSaved ( data() );
}

/**
* Kontext Menü für das Editieren
*/
void CommandLineEdit::contextMenuEvent ( QContextMenuEvent * e )
{
  QMenu* m = new QMenu ( this );

  QAction* ac_save = m->addAction ( getThemeIcon ( "document-save" ), trUtf8 ( "Save" ) );
  ac_save->setToolTip ( trUtf8 ( "Save current command list" ) );
  ac_save->setStatusTip ( trUtf8 ( "Save current command list" ) );
  connect ( ac_save, SIGNAL ( triggered() ), this, SLOT ( save() ) );

  QAction* ac_cpb = m->addAction ( getThemeIcon ( "edit-copy" ), trUtf8 ( "Copy to clipboard" ) );
  ac_cpb->setToolTip ( trUtf8 ( "Copy command list to Clipboard" ) );
  ac_cpb->setStatusTip ( trUtf8 ( "Copy command list to Clipboard" ) );
  connect ( ac_cpb, SIGNAL ( triggered() ), this, SLOT ( clipper() ) );

  m->exec ( e->globalPos() );
}

const QStringList CommandLineEdit::data ()
{
  QStringList data;
  for ( int r = 0; r < count(); r ++ )
  {
    QString cmd = item ( r )->text();
    if ( cmd.isEmpty() )
      continue;

    data << cmd.split ( " " );
  }
  return data;
}

void CommandLineEdit::setData ( const QStringList &cmdList )
{
  clear();
  if ( cmdList.size() > 1 )
  {
    foreach ( QString cmd, cmdList )
    {
      if ( cmd.isEmpty() )
        continue;

      QListWidgetItem* item = new QListWidgetItem ( cmd, this, QListWidgetItem::UserType );
      // Verhindere das verschieben der Binärdatei
      if ( cmd.contains ( QRegExp ( "\\bffmpeg\\b" ) ) )
        item->setFlags ( ( item->flags() & ~Qt::ItemIsDragEnabled ) );
      else
        item->setFlags ( ( item->flags() | Qt::ItemIsEditable ) );

      addItem ( item );
    }
  }
}

CommandLineEdit::~CommandLineEdit()
{}
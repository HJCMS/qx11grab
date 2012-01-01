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

#include "commandlineedit.h"
#include "createcustomitem.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QAbstractItemView>
#include <QtGui/QClipboard>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMenu>
#include <QtGui/QPalette>

CommandLineEdit::CommandLineEdit ( QWidget * parent )
    : QListWidget ( parent )
{
  setObjectName ( "commandlineedit" );
  setOptions();
}

/**
* FIXME Qt lost Options behind remove/insert actions!
*/
void CommandLineEdit::setOptions()
{
  setSortingEnabled ( false );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setBackgroundRole ( QPalette::AlternateBase );
  setDragDropMode ( QAbstractItemView::InternalMove );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setDragDropOverwriteMode ( false );
  setAlternatingRowColors ( true );
}

/**
* Einen neuen Eintrag erstellen!
*/
void CommandLineEdit::createCustomItem()
{
  CreateCustomItem* d = new CreateCustomItem ( this );
  if ( d->exec() == QDialog::Accepted )
  {
    if ( ! d->lineEdit->text().isEmpty() )
    {
      int r = ( count() - 1 );
      if ( r < 1 )
        return;

      clearSelection();
      insertItem ( r, d->lineEdit->text() );
      setCurrentRow ( r, QItemSelectionModel::Select );
      setOptions();
    }
  }
  delete d;
}

/**
* Kopiere den aktuellen Listen Inhalt ins das Clipboard
*/
void CommandLineEdit::clipper()
{
  QApplication::clipboard()->setText ( data().join ( " " ) );
}

/**
* Eine Zeile entfernen
*/
void CommandLineEdit::remove()
{
  QListWidgetItem* item = currentItem();
  if ( item )
  {
    clearSelection();
    removeItemWidget ( item );
    delete item;
  }
  setOptions();
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

  QAction* ac_save = m->addAction ( QIcon::fromTheme ( "document-save" ), trUtf8 ( "Save" ) );
  /*: ToolTip */
  ac_save->setToolTip ( trUtf8 ( "Save current command list" ) );
  ac_save->setStatusTip ( trUtf8 ( "Save current command list" ) );
  connect ( ac_save, SIGNAL ( triggered() ), this, SLOT ( save() ) );

  QAction* ac_remove = m->addAction ( QIcon::fromTheme ( "list-remove" ), trUtf8 ( "Remove" ) );
  /*: ToolTip */
  ac_remove->setToolTip ( trUtf8 ( "Remove this Command" ) );
  ac_remove->setStatusTip ( trUtf8 ( "Remove this Command" ) );
  connect ( ac_remove, SIGNAL ( triggered() ), this, SLOT ( remove() ) );

  QAction* ac_custom = m->addAction ( QIcon::fromTheme ( "list-add" ), trUtf8 ( "Insert" ) );
  /*: ToolTip */
  ac_custom->setToolTip ( trUtf8 ( "Create custom entry" ) );
  ac_custom->setStatusTip ( trUtf8 ( "Create custom entry" ) );
  connect ( ac_custom, SIGNAL ( triggered() ), this, SLOT ( createCustomItem() ) );

  QAction* ac_refresh = m->addAction ( QIcon::fromTheme ( "edit-undo" ), trUtf8 ( "Reset" ) );
  /*: ToolTip */
  ac_refresh->setToolTip ( trUtf8 ( "Restore Preview" ) );
  connect ( ac_refresh, SIGNAL ( triggered() ), this, SIGNAL ( restoreRequest() ) );

  QAction* ac_cpb = m->addAction ( QIcon::fromTheme ( "edit-copy" ), trUtf8 ( "Copy to clipboard" ) );
  /*: ToolTip */
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

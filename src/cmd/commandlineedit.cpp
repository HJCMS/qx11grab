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
* Qt verliert nach Entfernen/Hinzufügen von Einträgen
* die Optionen des Listenfeldes!
*/
void CommandLineEdit::setOptions()
{
  setSortingEnabled ( false );
  setDragDropOverwriteMode ( false );
  setAlternatingRowColors ( true );
  setSelectionMode ( QAbstractItemView::SingleSelection );
  setBackgroundRole ( QPalette::AlternateBase );
  setDragDropMode ( QAbstractItemView::InternalMove );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
}

/**
* Wenn Einträge erstellt wurden müssen die Flags neu gesetzt werden!
* \li Dabei wird die Zeile mit dem Programm geschützt!
* \li Die Zeile mit der Ausgabedatei wird vor verschieben geschützt!
* \li Alle anderen Zeilen bekommen ein Editieren erlaubt verpasst!
*/
void CommandLineEdit::setItemsFlags()
{
  QRegExp patternFirst ( "\\b(ffmpeg|avconv)\\b" );
  QRegExp patternLast ( "^\\-y[\\s\\t]+" );

  for ( int r = 0; r < count(); r++ )
  {
    if ( item ( r )->data ( Qt::DisplayRole ).toString().contains ( patternFirst ) )
      item ( r )->setFlags ( Qt::NoItemFlags );
    else if ( item ( r )->data ( Qt::DisplayRole ).toString().contains ( patternLast ) )
      item ( r )->setFlags ( ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable ) );
    else
      item ( r )->setFlags ( ( item ( r )->flags() | Qt::ItemIsEditable ) );
  }
}

/**
* Einen neuen Eintrag erstellen!
* Es muss ein Puffer angelegt werden weil qt4 wegen
* den Schreibgeschützten Flags eine vererbung generiert
* die hier nicht erwünscht ist!
*/
void CommandLineEdit::insertCustomItem ( const QString &data )
{
  int index = currentRow();  // NOTE !!! Zeile 0 ist blokiert !!!
  QStringList buffer;
  for ( int r = 0; r < count(); r++ )
  {
    if ( r == index )
      buffer.append ( data );

    buffer.append ( item ( r )->data ( Qt::DisplayRole ).toString() );
  }

  clear();
  insertItems ( 0, buffer );
  buffer.clear();
  setCurrentRow ( index, QItemSelectionModel::ClearAndSelect );
  setItemsFlags();
  setOptions();
}

/**
* Öffnet den Dialog für das Hinzufügen eines neuen Eintrages!
*/
void CommandLineEdit::createCustomItem()
{
  CreateCustomItem* d = new CreateCustomItem ( this );
  if ( d->exec() == QDialog::Accepted )
  {
    QString entry = d->lineEdit->text();
    if ( ! entry.isEmpty() && entry.contains ( QRegExp ( "^\\-" ) ) )
      insertCustomItem ( d->lineEdit->text() );
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

/**
* Den Kompletten Inhalt ausgeben!
*/
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

/**
* Liste erstellen!
*/
void CommandLineEdit::setData ( const QStringList &cmdList )
{
  clear();
  if ( cmdList.size() > 1 )
  {
    int r = 0;
    foreach ( QString cmd, cmdList )
    {
      if ( cmd.isEmpty() )
        continue;

      insertItem ( r++, cmd );
    }
    setItemsFlags();
  }
}

CommandLineEdit::~CommandLineEdit()
{}

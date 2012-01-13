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

#include "extraoptions.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QTableWidgetItem>

ExtraOptions::ExtraOptions ( QWidget * parent )
    : QTableWidget ( parent )
{
  setObjectName ( QLatin1String ( "ExtraOptions" ) );
  setToolTip ( trUtf8 ( "Expert Options" ) );
  setWhatsThis ( trUtf8 ( "Here you can configure extra commands." ) );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setWordWrap ( false );
  setCornerButtonEnabled ( true );
  setDragEnabled ( false );
  setDragDropOverwriteMode ( false );
  setDefaultDropAction ( Qt::MoveAction );
  setAlternatingRowColors ( true );
  setSelectionMode ( QAbstractItemView::ExtendedSelection );
  setSelectionBehavior ( QAbstractItemView::SelectRows );
  setContextMenuPolicy ( Qt::DefaultContextMenu );
  setGridStyle ( Qt::DashLine );
  setWordWrap ( false );
  setCornerButtonEnabled ( true );
  setColumnCount ( 2 );
  setRowCount ( 1 );

  QStringList headers;
  headers << trUtf8 ( "Parameter" ) << trUtf8 ( "Value" );
  setHorizontalHeaderLabels ( headers );

  /* Zellen anpassen */
  QHeaderView* tHeader = horizontalHeader();
  tHeader->setCascadingSectionResizes ( true );
  tHeader->setDefaultAlignment ( Qt::AlignLeft );
  tHeader->setMinimumSectionSize ( 110 );
  tHeader->setDefaultSectionSize ( 150 );
  tHeader->setStretchLastSection ( true );
  tHeader->setMovable ( false );

  // Kontext Menü
  m_menu = new QMenu ( this );
  /*: MenuEntry */
  QAction* m_actionInsert = m_menu->addAction ( Settings::themeIcon ( "insert-table" ), trUtf8 ( "Add" ) );
  /*: ToolTip */
  m_actionInsert->setToolTip ( trUtf8 ( "This button insert a new empty table row" ) );
  /*: MenuEntry */
  QAction* m_actionRemove = m_menu->addAction ( Settings::themeIcon ( "edit-table-delete-row" ), trUtf8 ( "Remove" ) );
  /*: ToolTip */
  m_actionRemove->setToolTip ( trUtf8 ( "This button remove selected table rows" ) );
  /*: MenuEntry */
  QAction* m_actionClear = m_menu->addAction ( Settings::themeIcon ( "edit-clear" ), trUtf8 ( "Clear" ) );
  /*: ToolTip */
  m_actionClear->setToolTip ( trUtf8 ( "This button clear table contents" ) );

  connect ( m_actionInsert, SIGNAL ( triggered () ), this, SLOT ( addTableRow() ) );
  connect ( m_actionRemove, SIGNAL ( triggered () ), this, SLOT ( delTableRow() ) );
  connect ( m_actionClear, SIGNAL ( triggered () ), this, SLOT ( clearContents() ) );
}

/**
* Eine neue Zeile in die Tabelle einfügen.
*/
void ExtraOptions::addTableRow()
{
  setRowCount ( ( rowCount() + 1 ) );
}

/**
* Ausgewählte Zeile aus Tabelle entfernen.
*/
void ExtraOptions::delTableRow()
{
  QList<int> rows;
  // erst nach gültigen zeilen suchen
  foreach ( QTableWidgetItem* item, selectedItems () )
  {
    if ( ! rows.contains ( item->row() ) )
      rows.append ( item->row() );
  }
  // jetzt Zeilen löschen
  foreach ( int r, rows )
  {
    removeRow ( r );
  }
}

/**
* Kontext Menü öffnen
*/
void ExtraOptions::contextMenuEvent ( QContextMenuEvent *e )
{
  m_menu->exec ( e->globalPos() );
}

void ExtraOptions::load ( Settings * cfg )
{
  const QHash<QString,QVariant> data = cfg->readGroup ( QLatin1String ( "ExpertOptions" ) );
  if ( data.size() > 0 )
  {
    int row = 0;
    clearContents();
    setRowCount ( ( data.size() + 1 ) );
    QHashIterator<QString,QVariant> it ( data );
    while ( it.hasNext() )
    {
      it.next();
      setItem ( row, 0, new QTableWidgetItem ( it.key(), QTableWidgetItem::UserType ) );
      setItem ( row, 1, new QTableWidgetItem ( it.value().toString(), QTableWidgetItem::UserType ) );
      row++;
    }
  }
}

void ExtraOptions::save ( Settings * cfg )
{
  QHash<QString,QVariant> data;
  for ( int r = 0; r < rowCount(); ++r )
  {
    if ( item ( r, 0 ) )
    {
      QString key = item ( r, 0 )->data ( Qt::EditRole ).toString();
      QVariant value = item ( r, 1 )->data ( Qt::EditRole );
      if ( key.isEmpty() )
        continue;

      data.insert ( key, value );
    }
  }
  cfg->saveGroup ( "ExpertOptions", data );
}

ExtraOptions::~ExtraOptions()
{}

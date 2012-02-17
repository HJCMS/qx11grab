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

#include "extensiontable.h"
#include "extensiontablemodel.h"

/* QX11Grab */
#include "avoptions.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QTableWidgetItem>

ExtensionTable::ExtensionTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "ExtensionTable" ) );
  setToolTip ( trUtf8 ( "Video File extensions" ) );
  setWhatsThis ( trUtf8 ( "here you can configure video file extensions" ) );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setWordWrap ( false );
  setCornerButtonEnabled ( true );
  setDragEnabled ( false );
  setDragDropOverwriteMode ( false );
  setAlternatingRowColors ( true );
  setSelectionMode ( QAbstractItemView::ExtendedSelection );
  setSelectionBehavior ( QAbstractItemView::SelectRows );
  setGridStyle ( Qt::DashLine );
  setCornerButtonEnabled ( true );

  /* Tabellen Model setzen */
  m_model = new ExtensionTableModel ( this );
  setModel ( m_model );

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
  QAction* m_actionInsert = m_menu->addAction ( QIcon::fromTheme ( "insert-table" ), trUtf8 ( "Add" ) );
  /*: ToolTip */
  m_actionInsert->setToolTip ( trUtf8 ( "This button insert a new empty table row" ) );
  /*: MenuEntry */
  QAction* m_actionRemove = m_menu->addAction ( QIcon::fromTheme ( "edit-table-delete-row" ), trUtf8 ( "Remove" ) );
  /*: ToolTip */
  m_actionRemove->setToolTip ( trUtf8 ( "This button remove selected table rows" ) );
  /*: MenuEntry */
  QAction* m_actionClear = m_menu->addAction ( QIcon::fromTheme ( "edit-clear" ), trUtf8 ( "Clear" ) );
  /*: ToolTip */
  m_actionClear->setToolTip ( trUtf8 ( "This button clear table contents" ) );

  connect ( m_actionInsert, SIGNAL ( triggered () ), this, SLOT ( addTableRow() ) );
  connect ( m_actionRemove, SIGNAL ( triggered () ), this, SLOT ( delTableRow() ) );
  connect ( m_actionClear, SIGNAL ( triggered () ), m_model, SLOT ( clearContents() ) );
}

/**
* Eine neue Zeile in die Tabelle einfügen.
*/
void ExtensionTable::addTableRow()
{
  QModelIndex index;
  m_model->insertRows ( ( m_model->rowCount ( index ) + 1 ), 1, index );
}

/**
* Ausgewählte Zeile aus Tabelle entfernen.
*/
void ExtensionTable::delTableRow()
{
}

/**
* Kontext Menü öffnen
*/
void ExtensionTable::contextMenuEvent ( QContextMenuEvent *e )
{
  m_menu->exec ( e->globalPos() );
}

void ExtensionTable::openFormats ( QSettings * settings )
{
  Q_UNUSED ( settings );

//   QX11Grab::AVOptions* avopts = new  QX11Grab::AVOptions( this );
//   avopts->codecFormats();

//   if ( data.size() > 0 )
//   {
//     int row = 0;
//     m_model->clearContents();
//     setRowCount ( ( data.size() + 1 ) );
//     QHashIterator<QString,QVariant> it ( data );
//     while ( it.hasNext() )
//     {
//       it.next();
//       setItem ( row, 0, new QTableWidgetItem ( it.key(), QTableWidgetItem::UserType ) );
//       setItem ( row, 1, new QTableWidgetItem ( it.value().toString(), QTableWidgetItem::UserType ) );
//       row++;
//     }
//   }
}

ExtensionTable::~ExtensionTable()
{}

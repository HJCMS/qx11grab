/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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
#include <QtGui/QVBoxLayout>

ExtraOptions::ExtraOptions ( QWidget * parent )
    : AbstractConfigWidget ( parent )
{
  setObjectName ( QLatin1String ( "ExtraOptions" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "Expert Options" ) );
  setWhatsThis ( trUtf8 ( "Here you can configure extra commands." ) );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 5, 0, 0 );
  layout->setObjectName ( QLatin1String ( "ExtraOptions/Layout" ) );

  m_tableWidget = new QTableWidget ( this );
  layout->addWidget ( m_tableWidget );

  m_tableWidget->setEditTriggers ( QAbstractItemView::DoubleClicked );
  m_tableWidget->setWordWrap ( false );
  m_tableWidget->setCornerButtonEnabled ( true );
  m_tableWidget->setDragEnabled ( false );
  m_tableWidget->setDragDropOverwriteMode ( false );
  m_tableWidget->setDefaultDropAction ( Qt::MoveAction );
  m_tableWidget->setAlternatingRowColors ( true );
  m_tableWidget->setSelectionMode ( QAbstractItemView::ExtendedSelection );
  m_tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows );
  m_tableWidget->setContextMenuPolicy ( Qt::DefaultContextMenu );
  m_tableWidget->setGridStyle ( Qt::DashLine );
  m_tableWidget->setWordWrap ( false );
  m_tableWidget->setCornerButtonEnabled ( true );
  m_tableWidget->setColumnCount ( 2 );
  m_tableWidget->setRowCount ( 1 );

  QStringList headers;
  headers << trUtf8 ( "Parameter" ) << trUtf8 ( "Value" );
  m_tableWidget->setHorizontalHeaderLabels ( headers );

  /* Zellen anpassen */
  QHeaderView* tHeader = m_tableWidget->horizontalHeader();
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

  setLayout ( layout );

  connect ( m_actionInsert, SIGNAL ( triggered () ), this, SLOT ( addTableRow() ) );
  connect ( m_actionRemove, SIGNAL ( triggered () ), this, SLOT ( delTableRow() ) );
  connect ( m_actionClear, SIGNAL ( triggered () ), m_tableWidget, SLOT ( clearContents() ) );
}

/**
* Eine neue Zeile in die Tabelle einfügen.
*/
void ExtraOptions::addTableRow()
{
  m_tableWidget->setRowCount ( ( m_tableWidget->rowCount() + 1 ) );
  emit postUpdate ( true );
}

/**
* Ausgewählte Zeile aus Tabelle entfernen.
*/
void ExtraOptions::delTableRow()
{
  QList<int> rows;
  // erst nach gültigen zeilen suchen
  foreach ( QTableWidgetItem* item, m_tableWidget->selectedItems () )
  {
    if ( ! rows.contains ( item->row() ) )
      rows.append ( item->row() );
  }
  // jetzt Zeilen löschen
  foreach ( int r, rows )
  {
    m_tableWidget->removeRow ( r );
  }
  if ( rows.size() > 0 )
    emit postUpdate ( true );
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
    m_tableWidget->clearContents();
    m_tableWidget->setRowCount ( ( data.size() + 1 ) );
    QHashIterator<QString,QVariant> it ( data );
    while ( it.hasNext() )
    {
      it.next();
      m_tableWidget->setItem ( row, 0, new QTableWidgetItem ( it.key(), QTableWidgetItem::UserType ) );
      m_tableWidget->setItem ( row, 1, new QTableWidgetItem ( it.value().toString(), QTableWidgetItem::UserType ) );
      row++;
    }
  }
}

void ExtraOptions::save ( Settings * cfg )
{
  QHash<QString,QVariant> data;
  for ( int r = 0; r < m_tableWidget->rowCount(); ++r )
  {
    if ( m_tableWidget->item ( r, 0 ) )
    {
      QString key = m_tableWidget->item ( r, 0 )->data ( Qt::EditRole ).toString();
      QVariant value = m_tableWidget->item ( r, 1 )->data ( Qt::EditRole );
      if ( key.isEmpty() )
        continue;

      data.insert ( key, value );
    }
  }
  cfg->saveGroup ( "ExpertOptions", data );
}

ExtraOptions::~ExtraOptions()
{}

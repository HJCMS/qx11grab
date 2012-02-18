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

#include "codectable.h"
#include "codectablemodel.h"
#include "codectabledelegate.h"
#include "filtermenu.h"
#include "settings.h"
#include "codecoptions.h"

/* QX11Grab interface */
#include "interface.h"
#include "qx11grabplugins.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QAction>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QMenu>

CodecTable::CodecTable ( QWidget * parent )
    : QTableView ( parent )
    , m_model ( 0 )
    , m_codecTableDelegate ( 0 )
{
  setObjectName ( QLatin1String ( "CodecTable" ) );
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
  m_model = new CodecTableModel ( this );
  setModel ( m_model );

  /* Tabellen Inhalts abfragen verfolgung setzen */
  m_codecTableDelegate = new CodecTableDelegate ( this );
  setItemDelegate ( m_codecTableDelegate );

  /* Zellen anpassen */
  QHeaderView* tHeader = horizontalHeader();
  tHeader->setCascadingSectionResizes ( true );
  tHeader->setDefaultAlignment ( Qt::AlignLeft );
  tHeader->setMinimumSectionSize ( 110 );
  tHeader->setDefaultSectionSize ( 150 );
  tHeader->setStretchLastSection ( true );
  tHeader->setMovable ( false );

  connect ( m_model, SIGNAL ( postUpdate() ),
            this, SIGNAL ( postUpdate() ) );
}

/** Einen Filter Dialog öffnen */
void CodecTable::openFilterDialog ( const QString &filter )
{
  QX11Grab::Plugins* m_plugins = new QX11Grab::Plugins ( this );
  if ( m_plugins )
  {
    QX11Grab::Interface* plugin = m_plugins->get ( filter, this );
    if ( plugin )
    {
      if ( plugin->exec() )
      {
        m_model->addOption ( rowCount(), "-filter:v", plugin->data() );
        emit postUpdate();
      }
      delete plugin;
    }
  }
  delete m_plugins;
}

/** Kontext Menü abfangen und einen Eingabe Dialoge anbieten */
void CodecTable::contextMenuEvent ( QContextMenuEvent * e )
{
  QMenu* m = new QMenu ( this );

  // OptionsMenu {
  QX11Grab::CodecOptions* com = new QX11Grab::CodecOptions ( this );
  connect ( com, SIGNAL ( optionChanged ( const QString &, const QVariant & ) ),
            this, SLOT ( appendItem ( const QString &, const QVariant & ) ) );
  m->addMenu ( com );
  // } OptionsMenu

  // FilterMenu {
  FilterMenu* mfm = new FilterMenu ( this );
  connect ( mfm, SIGNAL ( openFilter ( const QString & ) ),
            this, SLOT ( openFilterDialog ( const QString & ) ) );
  m->addMenu ( mfm );
  // } FilterMenu

  // Tabellen Aktionen {
  QAction* ac_add = m->addAction ( Settings::themeIcon ( "insert-table" ), trUtf8 ( "Add" ) );
  connect ( ac_add, SIGNAL ( triggered() ), this, SIGNAL ( insertEmptyRow() ) );

  QAction* ac_remove = m->addAction ( Settings::themeIcon ( "edit-table-delete-row" ), trUtf8 ( "Remove" ) );
  ac_remove->setEnabled ( ( selectedIndexes().size() > 0 ) ); // Nur aktiv wenn Zeile ausgewählt ist
  connect ( ac_remove, SIGNAL ( triggered() ), this, SIGNAL ( removeSelectedRow() ) );

  QAction* ac_clear = m->addAction ( Settings::themeIcon ( "edit-clear" ), trUtf8 ( "Clear" ) );
  connect ( ac_clear, SIGNAL ( triggered() ), this, SLOT ( clearContents() ) );
  // } Tabellen Aktionen

  m->exec ( e->globalPos() );
  delete m;
}

/** Komplette Tabelle leeren */
void CodecTable::clearContents()
{
  return m_model->clear();
}

/** Datensatz anhängen */
void CodecTable::appendItem ( const QString &key, const QVariant &value )
{
  m_model->addOption ( m_model->rowCount(), key, value );
}

/** Datensatz einfügen */
void CodecTable::insertItem ( int row, const QString &key, const QVariant &value )
{
  m_model->addOption ( row, key, value );
}

/** Zeilen Anzahl */
int CodecTable::rowCount()
{
  return m_model->rowCount();
}

/** Spalten Anzahl */
int CodecTable::columnCount()
{
  return m_model->columnCount();
}

/** Den Inhalt einer Ausgewählten Zeile zurück geben */
const QPair<QString,QVariant> CodecTable::item ( int row )
{
  return m_model->option ( row );
}

/** Gibt den Inhalt einer Spalte zurück */
const QVariant CodecTable::item ( int row, int column )
{
  QPair<QString,QVariant> p = m_model->option ( row );
  if ( column == 0 )
    return p.first;
  else
    return p.second;
}

/** Ausgewählte Zeile entfernen */
void CodecTable::removeRow ( int row )
{
  m_model->removeRows ( row, 1 );
}

/** Neue Zeile an Position einfügen */
void CodecTable::insertRow ( int row )
{
  m_model->insertRows ( row, 1 );
}

/** Gibt die ausgewählten Zeilen zurück */
const QList<int> CodecTable::selectedRows()
{
  QList<int> rows;
  QModelIndexList m = selectedIndexes();
  for ( int i = 0; i < m.size(); ++i )
  {
    if ( m.at ( i ).column() == 0 )
      rows.append ( m.at ( i ).row() );
  }
  return rows;
}

CodecTable::~CodecTable()
{}

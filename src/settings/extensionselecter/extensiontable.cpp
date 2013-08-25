/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "extensiontable.h"
#include "extensiontablemodel.h"

/* QX11Grab */
#include "avoptions.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QAction>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QItemSelectionModel>

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
  // FIXME Qt4 hat Probleme bei einem entfernen mehrerer Models die Indexes zur laufzeit richtig zus setzen!
  setSelectionMode ( QAbstractItemView::SingleSelection );
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
  QModelIndexList m = selectedIndexes();
  for ( int i = 0; i < m.size(); ++i )
  {
    if ( m.at ( i ).column() == 0 )
      m_model->removeRows ( m.at ( i ).row(), 1 );
  }
}

/**
* Kontext Menü öffnen
*/
void ExtensionTable::contextMenuEvent ( QContextMenuEvent *e )
{
  m_menu->exec ( e->globalPos() );
}

/**
* Lade alle Erweiterungen in die Tabelle
* \li Sehe nach ob schon eine Konfiguration existiert,
* \li Wenn ja lese aus der Konfiguration und befülle Tabelle,
* \li Wenn keine Konfiguration dann nehme FFmpeg Standards
*/
void ExtensionTable::load ( QSettings * settings )
{
  int index = 0;
  QHash<int,ExtensionTableModel::ExtensionTableItem> items;
  // Wenn in den Settings vorhanden!
  int size = settings->beginReadArray ( "CodecExtensions" );
  if ( size > 0 )
  {
    for ( int i = 0; i < size; ++i )
    {
      settings->setArrayIndex ( i );
      ExtensionTableModel::ExtensionTableItem item;
      item.format = settings->value ( "format" ).toString();
      item.defaultExt = settings->value ( "defaultExt" ).toString();
      item.extensions = settings->value ( "extensions" ).toStringList();
      items.insert ( i, item );
    }
  }
  settings->endArray(); // !! Muss immmer geschlossen werden !!

  // Ready to insert ??
  if ( items.size() > 0 )
  {
    m_model->insertItems ( items );
    return;
  }

  QList<QX11Grab::FFCodec> codecList = QX11Grab::AVOptions::videoCodecs();
  if ( codecList.size() > 0 )
  {
    for ( int i = 0; i < codecList.size(); ++i )
    {
      if ( codecList.at ( i ).id != AV_CODEC_ID_NONE )
      {
        QStringList buffer;
        QList<QX11Grab::FFFormat> extsList = QX11Grab::AVOptions::supportedFormats ( codecList.at ( i ).id );
        if ( extsList.size() > 0 )
        {
          for ( int j = 0; j < extsList.size(); ++j )
          {
            QStringList l = extsList.at ( j ).extensions.toStringList();
            buffer.append ( l );
          }
        }
        buffer.removeDuplicates();
        if ( buffer.size() > 0 )
        {
          ExtensionTableModel::ExtensionTableItem item;
          item.format = codecList.at ( i ).name;
          item.defaultExt = buffer.first();
          item.extensions = buffer;
          // qDebug() << Q_FUNC_INFO << item.format << buffer;
          items.insert ( index++, item );
        }
      }
    }
    // Ready to insert ??
    if ( items.size() > 0 )
      m_model->insertItems ( items );
  }
}

/**
* Lese Aktuellen Tabellen Inhalt und schreibe in die Konfiguration
*/
void ExtensionTable::save ( QSettings * settings )
{
  QHash<int,ExtensionTableModel::ExtensionTableItem> items = m_model->modelItems();
  settings->remove ( "CodecExtensions" );
  settings->beginWriteArray ( "CodecExtensions", items.size() );
  for ( int i = 0; i < items.size(); ++i )
  {
    settings->setArrayIndex ( i );
    settings->setValue ( "format", items.value ( i ).format );
    settings->setValue ( "defaultExt", items.value ( i ).defaultExt );
    settings->setValue ( "extensions", items.value ( i ).extensions );
  }
  settings->endArray();
}

ExtensionTable::~ExtensionTable()
{}

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

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QTableWidgetItem>

CodecTable::CodecTable ( QWidget * parent )
    : QTableView ( parent )
{
  setObjectName ( QLatin1String ( "CodecTable" ) );
  setEditTriggers ( QAbstractItemView::DoubleClicked );
  setWordWrap ( false );
  setCornerButtonEnabled ( true );
  setDragEnabled ( false );
  setDragDropOverwriteMode ( false );
  setDefaultDropAction ( Qt::MoveAction );
  setAlternatingRowColors ( true );
  setSelectionMode ( QAbstractItemView::ExtendedSelection );
  setSelectionBehavior ( QAbstractItemView::SelectRows );
  setGridStyle ( Qt::DashLine );
  setWordWrap ( false );
  setCornerButtonEnabled ( true );

  m_model = new CodecTableModel ( this );
  setModel ( m_model );

  /* Zellen anpassen */
  QHeaderView* tHeader = horizontalHeader();
  tHeader->setCascadingSectionResizes ( true );
  tHeader->setDefaultAlignment ( Qt::AlignLeft );
  tHeader->setMinimumSectionSize ( 110 );
  tHeader->setDefaultSectionSize ( 150 );
  tHeader->setStretchLastSection ( true );
  tHeader->setMovable ( false );
}

void CodecTable::clearContents()
{
  return m_model->clear();
}

int CodecTable::rowCount()
{
  return m_model->rowCount();
}

int CodecTable::columnCount()
{
  return m_model->columnCount();
}

void CodecTable::insertItem ( int row, const QString &key, const QVariant &value )
{
  m_model->addOption ( row, key, value );
}

const QPair<QString,QVariant> CodecTable::item ( int row )
{
  return m_model->option ( row );
}

const QVariant CodecTable::item ( int row, int column )
{
  QPair<QString,QVariant> p = m_model->option ( row );
  if ( column == 0 )
    return p.first;
  else
    return p.second;
}

void CodecTable::removeRow ( int row )
{
  m_model->removeRows ( row, 1 );
}

void CodecTable::insertRow ( int row )
{
  m_model->insertRows ( row, 1 );
}

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

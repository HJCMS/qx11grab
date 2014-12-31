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

#include "extensiontablemodel.h"
#include "extensiondelegation.h"

/* QtCore */
#include <QtCore/QDebug>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>

ExtensionTableModel::ExtensionTableModel ( QTableView * parent )
    : QAbstractTableModel ( parent )
{
  setObjectName ( QLatin1String ( "ExtensionTableModel" ) );
  items.clear();

  QHeaderView* header = parent->horizontalHeader ();
  header->setResizeMode ( QHeaderView::ResizeToContents );
  header->setMovable ( false );
  header->setDefaultAlignment ( Qt::AlignLeft );
  header->setStretchLastSection ( true );

  parent->setItemDelegate ( new ExtensionDelegation ( parent ) );
}

/**
* Entfernt einen Eintrag und sortiert die
* Tabellen Indexes für den Vector neu.
* \note Qt bekommt es im Moment nicht gebacken mehrere Einträge
*       Gleichzeitg zu löschen! Es werden zwar in dieser Methode
*       die Indexes neu gesetzt, aber Irgendwie kommt das nicht
*       beim Tabellen Model an :-/
*/
bool ExtensionTableModel::removeItem ( int key )
{
  if ( items.remove ( key ) == 1 )
  {
    int index = 0;
    QHash<int,ExtensionTableItem> copy;
    QHash<int,ExtensionTableItem>::iterator i;
    for ( i = items.begin(); i != items.end(); ++i )
    {
      // qDebug() << Q_FUNC_INFO << key << index << i.value().format;
      copy.insert ( index++, i.value() );
    }
    items.clear();
    items = copy;
    return true;
  }
  return false;
}

Qt::ItemFlags ExtensionTableModel::flags ( const QModelIndex &index ) const
{
  if ( ! index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags  = ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
  return flags;
}

QVariant ExtensionTableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  QVariant value;
  if ( ( orientation == Qt::Horizontal ) && section < 3 )
  {
    switch ( role )
    {
      case Qt::DisplayRole:
      {
        if ( section == 0 )
          return trUtf8 ( "Codec" );
        else if ( section == 1 )
          return trUtf8 ( "Default" );
        else
          return trUtf8 ( "Extensions" );
      }

      case Qt::DecorationRole:
        return QIcon::fromTheme ( "view-form-table" );

      case Qt::SizeHintRole:
        return QSize ( 110, 25 );

      default:
        return value;
    }
  }
  else if ( orientation == Qt::Vertical )
  {
    switch ( role )
    {
      case Qt::DisplayRole:
        return section;

      default:
        return value;
    };
  }
  return value;
}

QVariant ExtensionTableModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( ! index.isValid() )
    return val;

  if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) )
  {
    switch ( index.column() )
    {
      case 0:
        return items.value ( index.row() ).format;

      case 1:
        return items.value ( index.row() ).defaultExt;

      case 2:
      {
        if ( role == Qt::DisplayRole )
          return items.value ( index.row() ).extensions.join ( "," );
        else
          return items.value ( index.row() ).extensions;
      }

      default:
        return val;
    };
  }
  return val;
}

QModelIndex ExtensionTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > items.size() )
    return QModelIndex();

  if ( column > columnCount() )
    return QModelIndex();

  return createIndex ( row, column, items.size() );
}

QModelIndex ExtensionTableModel::parent ( const QModelIndex &index ) const
{
  if ( ! index.isValid() )
    return QModelIndex();

  return QModelIndex();
}

void ExtensionTableModel::clearContents()
{
  beginRemoveRows ( QModelIndex(), 0, items.size() );
  items.clear ();
  endRemoveRows();
}

int ExtensionTableModel::columnCount ( const QModelIndex & ) const
{
  return 3;
}

int ExtensionTableModel::rowCount ( const QModelIndex & ) const
{
  return items.size();
}

bool ExtensionTableModel::insertRows ( int row, int count, const QModelIndex &parent )
{
  bool status = false;
  beginInsertRows ( parent, items.size(), items.size() );
  ExtensionTableItem item;
  item.format = QString();
  item.defaultExt = QString();
  item.extensions = QStringList();
  for ( int i = row; i < ( row + count ); ++i )
  {
    items.insert ( i, item );
    status = true;
  }
  endInsertRows();
  return status;
}

bool ExtensionTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
{
  bool status = false;
  if ( ! index.isValid() )
    return status;

  if ( role == Qt::EditRole )
  {
    int row = index.row();
    if ( ( items.count ( row ) == 0 )
            && ! ( insertRows ( ( items.size() + 1 ), 1, index ) ) )
      return false;

    switch ( index.column() )
    {
      case 0:
      {
        QString data = value.toString();
        if ( ! data.isEmpty() )
        {
          ExtensionTableItem item = items.value ( row );
          item.format = data;
          items[row] = item;
          status = true;
        }
      }
      break;

      case 1:
      {
        ExtensionTableItem item = items.value ( row );
        item.defaultExt = value.toString();
        items[row] = item;
        status = true;
      }
      break;

      case 2:
      {
        ExtensionTableItem item = items.value ( row );
        item.extensions = value.toStringList();
        items[row] = item;
        status = true;
      }
      break;

      default:
        return false;
    };
  }

  return status;
}

bool ExtensionTableModel::removeRows ( int row,  int count, const QModelIndex &parent )
{
  Q_UNUSED ( count ); // Wir entfernen Maximal eine Zeile siehe removeItem

  bool status = false;
  if ( ( row < 0 ) || ( row > items.size() ) )
    return status;

  if ( items.count ( row ) == 0 )
    return status;

  beginRemoveRows ( parent, row, row );
  status = removeItem ( row );
  endRemoveRows();
  return status;
}

void ExtensionTableModel::insertItems ( const QHash<int,ExtensionTableModel::ExtensionTableItem> &data )
{
  if ( data.size() > 0 )
  {
    int index = 0;
    items.clear();
    beginInsertRows ( QModelIndex(), 0, data.size() );
    QHashIterator<int,ExtensionTableItem> it ( data );
    while ( it.hasNext() )
    {
      it.next();
      items.insert ( index++, it.value() );
    }
    endInsertRows();
  }
}

const QHash<int,ExtensionTableModel::ExtensionTableItem> ExtensionTableModel::modelItems()
{
  return items;
}

ExtensionTableModel::~ExtensionTableModel()
{
  if ( items.size() > 0 )
    items.clear();
}

/*
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
*/

#include "codectablemodel.h"
#include "settings.h"

/* QX11Grab */
#include "avoptions.h"
#include "avfiltermodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHashIterator>
#include <QtCore/QRegExp>
#include <QtCore/QSize>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

CodecTableModel::CodecTableModel ( QWidget * parent )
    : QAbstractTableModel ( parent )
{
  setObjectName ( QLatin1String ( "CodecTableModel" ) );
  items.clear();
}

/**
* Notwendig wegen tauschen von veralteten Argumenten
*/
const QString CodecTableModel::replaceDeprecated ( const QString &key ) const
{
  QString str ( key );
  str.trimmed();
  // ab sofort kein -vf mehr zulassen!
  if ( str.contains ( "-vf" ) )
    str.replace ( "-vf", QX11Grab::avFilterPredicate );
  else if ( str.contains ( "-af" ) )
    str.replace ( "-af", QX11Grab::acFilterPredicate );

  return str;
}

/**
* Entfernt einen Eintrag und sortiert die
* Tabellen Indexes für den Vector neu.
*/
bool CodecTableModel::removeItem ( int key )
{
  if ( items.remove ( key ) == 1 )
  {
    int index = 0;
    QHash<int,Item> copy;
    QHash<int,Item>::iterator i;
    for ( i = items.begin(); i != items.end(); ++i )
    {
      qDebug() << Q_FUNC_INFO << i.value().argument << "\n";
      copy.insert ( index++, i.value() );
    }
    items.clear();
    items = copy;
    emit postUpdate();
    return true;
  }
  return false;
}

/**
* Sucht mit der Parameterangabe nach einer Zeile
*/
int CodecTableModel::searchRow ( const QString &param )
{
  int ret = 0;
  QHashIterator<int,Item> it ( items );
  while ( it.hasNext() )
  {
    it.next();
    if ( it.value().argument == param )
      return it.key();
  }
  return ret;
}

/**
* Nachrichten an die Hauptfenster Statusleiste senden!
*/
void CodecTableModel::housemaster ( const QString &text )
{
  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  iface.call ( "message", text );
}

/**
* Wenn mit einem TableModel gearbeitet wird müssen die
* Editor flags neu gesetzt werden.
*/
Qt::ItemFlags CodecTableModel::flags ( const QModelIndex &index ) const
{
  if ( ! index.isValid() )
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags  = ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable );
  return flags;
}

bool CodecTableModel::setData ( const QModelIndex &index, const QVariant &value, int role )
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

#ifdef MAINTAINER_REPOSITORY
    qDebug() << Q_FUNC_INFO << index.row() << index.column() << value;
#endif

    switch ( index.column() )
    {
      case 0:
      {
        QString data = value.toString();
        if ( ! data.isEmpty() )
        {
          Item item = items.value ( row );
          item.argument =  replaceDeprecated ( data );
          items[row] = item;
          status = true;
        }
      }
      break;

      case 1:
      {
        QString data = value.toString();
        Item item = items.value ( row );
        /**
        * Ein Argument kann und darf keine Wert Zuweisung haben.
        * Aber für eine Wertzuweisung muss ein Argument immer vorhanden sein!
        */
        if ( item.argument.isEmpty() )
        {
          /*: StatusBar Message */
          housemaster ( trUtf8 ( "At first, we have to create a argument." ) );
          return false;
        }
        item.value = value;
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

int CodecTableModel::columnCount ( const QModelIndex & ) const
{
  return 2;
}

int CodecTableModel::rowCount ( const QModelIndex & ) const
{
  return items.size();
}

QVariant CodecTableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
  QVariant value;
  if ( ( orientation == Qt::Horizontal ) && section < 3 )
  {
    switch ( role )
    {
      case Qt::DisplayRole:
      {
        if ( section == 0 )
          return trUtf8 ( "Parameter" );
        else
          return trUtf8 ( "Value" );
      }

      case Qt::DecorationRole:
        return Settings::themeIcon ( "view-form-table" );

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
        break;

      default:
        return value;
    };
  }
  return value;
}

QVariant CodecTableModel::data ( const QModelIndex &index, int role ) const
{
  const QVariant val;
  if ( ! index.isValid() )
    return val;

  if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) )
  {
    switch ( index.column() )
    {
      case 0:
        return items.value ( index.row() ).argument;

      case 1:
        return items.value ( index.row() ).value;

      default:
        return val;
    };
  }
  return val;
}

QModelIndex CodecTableModel::index ( int row, int column, const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return QModelIndex();

  if ( row > items.size() )
    return QModelIndex();

  if ( column > columnCount() )
    return QModelIndex();

  return createIndex ( row, column, items.size() );
}

QModelIndex CodecTableModel::parent ( const QModelIndex &index ) const
{
  if ( ! index.isValid() )
    return QModelIndex();

  return QModelIndex();
}

/**
* Suche nach vorhandenen Filter Einträgen und ersetze diese bei bedarf!
*/
bool CodecTableModel::setFilterData ( int row, const QVariant &value, const QModelIndex &parent )
{
  QModelIndex mIndex = index ( row, 1, parent );
  // Aktuelle Filter finden und einlesen
  QString filters = data ( mIndex, Qt::EditRole ).toString();
  if ( filters.isEmpty() )
  {
    if ( setData ( mIndex, value, Qt::EditRole ) )
    {
      emit postUpdate();
      return true;
    }
    else
      return false;
  }

  QX11Grab::AVFilterModel* m_filterModel = new QX11Grab::AVFilterModel ( filters );
  m_filterModel->insertFilter ( value );
  bool status = setData ( mIndex, m_filterModel->values(), Qt::EditRole );
  delete m_filterModel;
  return status;
}

/** Einträge ab Zeile mit Anzahl index einfügen.   */
bool CodecTableModel::insertRows ( int row, int count, const QModelIndex &parent )
{
  bool status = false;
  beginInsertRows ( parent, items.size(), items.size() );
  Item item;
  item.argument = QString();
  item.value = QVariant();
  for ( int i = row; i < ( row + count ); ++i )
  {
    items.insert ( i, item );
    status = true;
  }
  endInsertRows();
  return status;
}

void CodecTableModel::clear()
{
  beginRemoveRows ( QModelIndex(), 0, items.size() );
  items.clear ();
  endRemoveRows();
  emit postUpdate();
}

/**
* Einen neuen Eintrag erzeugen.
* Hierbei sind folgenede Aktionen vorgschaltet:
* \li Suche nach vorhandenen Parameter in der Tabelle
* \li Wenn vorhanden den Werte Inhalt mit den Vorhandenen ersetzen!
* \li Ist es ein Filter?
* \li Dann die Werte aus der Vorhandenen Tabelle Zelle mit der neuen vergleichen und verbinden!
* \li Andernfalls wird mit beginInsertRows eine Neuer Eintrag erzeugt!
*/
void CodecTableModel::addOption ( int row, const QString &key, const QVariant &value,
                                  const QModelIndex &parent )
{
  if ( key.isEmpty() )
    return;

  QString predicate = replaceDeprecated ( key );
  // Suche nach Vorhandenen Datensatz und ersetze bei bedarf den Inhalt!
  int rindex = searchRow ( predicate );
  if ( rindex != 0 )
  {
    if ( predicate.contains ( QX11Grab::avFilterPredicate ) )
      setFilterData ( rindex, value, parent );
    else if ( predicate.contains ( QX11Grab::acFilterPredicate ) )
      setFilterData ( rindex, value, parent );
    else if ( value.isNull() )
      return; // Überschreiben mit leeren inhalt unterbinden
    else
      setData ( index ( rindex, 1, parent ), value, Qt::EditRole );

    return;
  }

  beginInsertRows ( parent, items.size(), items.size() );
  Item item;
  item.argument = predicate;
  item.value = value;
  items.insert ( row, item );
  endInsertRows();
}

bool CodecTableModel::removeRows ( int row,  int count, const QModelIndex &parent )
{
  Q_UNUSED ( count );

  bool status = false;
  if ( ( row < 0 ) || ( row > items.size() ) )
    return status;

  if ( items.count ( row ) == 0 )
    return status;

  beginRemoveRows ( parent, row, row );
  /**
  * WARNING Qt4:Bug Innerhalb von virtuellen Methoden auf Vector:Iteratoren zugreifen
  * ist keine gute Idee. Deshalb in eigene gekapselte Methode. \ref removeItem
  */
  status = removeItem ( row );
  endRemoveRows();
  return status;
}

/** Abfrage eines Eintrages mit Zeile und Index */
const QPair<QString,QVariant> CodecTableModel::option ( int row, const QModelIndex &parent )
{
  Q_UNUSED ( parent );
  QPair<QString,QVariant> p;
  if ( items.count ( row ) == 0 )
    return p;

  p.first = items.value ( row ).argument;
  p.second = items.value ( row ).value;
  return p;
}

/** Vector leeren */
CodecTableModel::~CodecTableModel()
{
  if ( items.size() > 0 )
    items.clear();
}

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

#ifndef EXTENSIONTABLEMODEL_H
#define EXTENSIONTABLEMODEL_H

/* QtCore */
#include <QtCore/QAbstractItemModel>
#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QTableView>
#include <QtGui/QWidget>

/* QX11Grab */
#include "avoptions.h"

class ExtensionTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  public:
    struct ExtensionTableItem
    {
      QString format;
      QString defaultExt;
      QStringList extensions;
    };

  private:
    QHash<int,ExtensionTableItem> items;
    bool removeItem ( int key );

  protected:
    Qt::ItemFlags flags ( const QModelIndex &index ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex &index ) const;

  public Q_SLOTS:
    void clearContents();

  public:
    explicit ExtensionTableModel ( QTableView * parent = 0 );
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    bool insertRows ( int row, int count, const QModelIndex &parent = QModelIndex() );
    bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
    bool removeRows ( int row,  int count, const QModelIndex &parent = QModelIndex() );
    void insertItems ( const QHash<int,ExtensionTableModel::ExtensionTableItem> & );
    const QHash<int,ExtensionTableModel::ExtensionTableItem> modelItems();
    virtual ~ExtensionTableModel();
};

#endif

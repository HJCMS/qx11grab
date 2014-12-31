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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public License
* along with this library; see the file COPYING.LIB. If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#ifndef CODECTABLEMODEL_H
#define CODECTABLEMODEL_H

/* QtCore */
#include <QtCore/QAbstractItemModel>
#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QWidget>

/* QX11Options */
#include "avoptions.h"

class CodecTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    struct Item
    {
      QString argument;
      QVariant value;
    };
    QHash<int,Item> items;
    const QString replaceDeprecated ( const QString & ) const;
    bool removeItem ( int key );
    int searchRow ( const QString &param );

  private Q_SLOTS:
    void housemaster ( const QString & );

  protected:
    Qt::ItemFlags flags ( const QModelIndex &index ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex &index ) const;
    bool setFilterData ( int row, const QVariant &value, const QModelIndex &parent = QModelIndex() );

  Q_SIGNALS:
    void postUpdate();

  public:
    explicit CodecTableModel ( QWidget * parent );
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    bool insertRows ( int row, int count, const QModelIndex &parent = QModelIndex() );
    bool setData ( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
    // Project Options
    void clear();
    void addOption ( int row, const QString &key, const QVariant &value, const QModelIndex &parent = QModelIndex() );
    bool removeRows ( int row,  int count, const QModelIndex &parent = QModelIndex() );
    const QPair<QString,QVariant> option ( int row, const QModelIndex &parent = QModelIndex() );
    virtual ~CodecTableModel();
};

#endif

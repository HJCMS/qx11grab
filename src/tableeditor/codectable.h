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

#ifndef CODECTABLE_H
#define CODECTABLE_H

/* QtCore */
#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QContextMenuEvent>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

class CodecTableModel;
class CodecTableDelegate;

class CodecTable : public QTableView
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )

  private:
    CodecTableModel* m_model;
    CodecTableDelegate* m_codecTableDelegate;

  private Q_SLOTS:
    void openFilterDialog ( const QString &filter );
    void appendItem ( const QString &key, const QVariant &value );

  protected:
    void contextMenuEvent ( QContextMenuEvent * );

  Q_SIGNALS:
    void postUpdate();
    void insertEmptyRow();
    void removeSelectedRow();

  public Q_SLOTS:
    void clearContents();
    void insertItem ( int row, const QString &key, const QVariant &value );

  public:
    explicit CodecTable ( QWidget * parent = 0 );

    /** table row count */
    int rowCount();

    /** table column count */
    int columnCount();

    /** get item with row */
    const QPair<QString,QVariant> item ( int row );

    /** get item with row and cell */
    const QVariant item ( int row, int column );

    /** remove row with row number */
    void removeRow ( int row );

    /** Insert a new behind row */
    void insertRow ( int row );

    /** currently selected row numbers */
    const QList<int> selectedRows();

    virtual ~CodecTable();
};

#endif

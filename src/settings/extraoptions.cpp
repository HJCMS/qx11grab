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

#include "extraoptions.h"

/* QX11Grab */
// #include ""

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QTableWidgetItem>

ExtraOptions::ExtraOptions ( QWidget * parent )
    : QTableWidget ( parent )
{
  setObjectName ( QLatin1String ( "ExtraOptions" ) );
  setToolTip ( trUtf8 ( "Expert Options" ) );
  setWhatsThis ( trUtf8 ( "Here you can configure extra commands." ) );
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
  setColumnCount ( 2 );
  setRowCount ( 1 );

  QStringList headers;
  headers << trUtf8 ( "Parameter" ) << trUtf8 ( "Value" );
  setHorizontalHeaderLabels ( headers );

  /* Zellen anpassen */
  QHeaderView* tHeader = horizontalHeader();
  tHeader->setCascadingSectionResizes ( true );
  tHeader->setDefaultAlignment ( Qt::AlignLeft );
  tHeader->setMinimumSectionSize ( 110 );
  tHeader->setDefaultSectionSize ( 150 );
  tHeader->setStretchLastSection ( true );
  tHeader->setMovable ( false );
}

void ExtraOptions::load ( Settings * cfg )
{
  const QHash<QString,QVariant> data = cfg->readGroup ( QLatin1String ( "ExpertOptions" ) );
  if ( data.size() > 0 )
  {
    int row = 0;
    clearContents();
    setRowCount ( 1 );
    QHashIterator<QString,QVariant> it ( data );
    while ( it.hasNext() )
    {
      it.next();
      // setRowCount ( row );
      setItem ( row, 0, new QTableWidgetItem ( it.key(), QTableWidgetItem::UserType ) );
      setItem ( row, 1, new QTableWidgetItem ( it.value().toString(), QTableWidgetItem::UserType ) );
      row++;
    }
  }
}

void ExtraOptions::save ( Settings * cfg )
{
  QHash<QString,QVariant> data;
  for ( int r = 0; r < rowCount(); ++r )
  {
    QString key = item ( r, 0 )->data ( Qt::EditRole ).toString();
    QVariant value = item ( r, 1 )->data ( Qt::EditRole );
    if ( key.isEmpty() )
      continue;

    data.insert ( key, value );
  }
  cfg->saveGroup ( "ExpertOptions", data );
}

ExtraOptions::~ExtraOptions()
{}

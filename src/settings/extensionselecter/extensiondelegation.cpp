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

#include "extensiondelegation.h"
#include "codecselecter.h"
#include "extensionscombobox.h"
#include "extensioncodeccombobox.h"
#include "extensionlineedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>

ExtensionDelegation::ExtensionDelegation ( QTableView * parent )
    : QItemDelegate ( parent )
{
  setObjectName ( QLatin1String ( "ExtensionDelegation" ) );
}

QWidget* ExtensionDelegation::createEditor ( QWidget * editor,
        const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  Q_UNUSED ( option );
  if ( index.column() == 0 )
    return new ExtensionCodecComboBox ( editor );
  else if ( index.column() == 1 )
    return new ExtensionLineEdit ( editor );
  else if ( index.column() == 2 )
    return new ExtensionsComboBox ( editor );
  else
    return new QLineEdit ( editor );
}

void ExtensionDelegation::setEditorData ( QWidget * editor, const QModelIndex &index ) const
{
  if ( index.column() == 0 )
  {
    ExtensionCodecComboBox* box = static_cast<ExtensionCodecComboBox*> ( editor );
    box->setCodec ( index.model()->data ( index ).toString() );
    return;
  }

  QString codec = index.sibling ( index.row(), 0 ).data().toString();
  if ( index.column() == 1 )
  {
    ExtensionLineEdit* box = static_cast<ExtensionLineEdit*> ( editor );
    box->setCompleterCodec ( codec );
    box->setText ( index.model()->data ( index ).toString() );
  }
  else if ( index.column() == 2 )
  {
    ExtensionsComboBox* box = static_cast<ExtensionsComboBox*> ( editor );
    QString current = index.model()->data ( index ).toString();
    if ( current.isEmpty() ) // Doppelte Einträge Verhindern !!!
      box->setCodec ( codec );
    else
      box->setText ( current );
  }
  else
  {
    QLineEdit* lineEdit = static_cast<QLineEdit*> ( editor );
    lineEdit->setText ( index.model()->data ( index ).toString() );
  }
}

void ExtensionDelegation::setModelData ( QWidget * editor,
        QAbstractItemModel *model, const QModelIndex &index ) const
{
  if ( index.column() == 0 )
  {
    ExtensionCodecComboBox* box = static_cast<ExtensionCodecComboBox*> ( editor );
    model->setData ( index, QVariant ( box->getCodec() ), Qt::EditRole );
  }
  else if ( index.column() == 1 )
  {
    ExtensionLineEdit* box = static_cast<ExtensionLineEdit*> ( editor );
    model->setData ( index, QVariant ( box->text() ), Qt::EditRole );
  }
  else if ( index.column() == 2 )
  {
    ExtensionsComboBox* box = static_cast<ExtensionsComboBox*> ( editor );
    model->setData ( index, QVariant ( box->data().join ( "," ) ), Qt::EditRole );
  }
  else
  {
    QLineEdit* sw = static_cast<QLineEdit*> ( editor );
    model->setData ( index, QVariant ( sw->text() ), Qt::EditRole );
  }
}

void ExtensionDelegation::updateEditorGeometry ( QWidget * editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index ) const
{
  Q_UNUSED ( index );
  editor->setGeometry ( option.rect );
  editor->setContentsMargins ( 0, 0, 0, 0 );
}

ExtensionDelegation::~ExtensionDelegation()
{}

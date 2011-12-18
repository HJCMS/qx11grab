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

#include "codectabledelegate.h"
#include "codectablemodel.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QLineEdit>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

CodecTableDelegate::CodecTableDelegate ( QObject * parent )
    : QItemDelegate ( parent )
{
  setObjectName ( QLatin1String ( "CodecTableDelegate" ) );
}

void CodecTableDelegate::housemaster ( const QString &message ) const
{
  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  iface.call ( "message", message );
}

QWidget* CodecTableDelegate::createEditor ( QWidget* parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index ) const
{
  Q_UNUSED ( option );
  QVariant data = index.model()->data ( index );
  QLineEdit* w = new QLineEdit ( parent );
  w->setText ( data.toString() );
  // qDebug() << Q_FUNC_INFO << w->text();
  return w;
}

void CodecTableDelegate::setEditorData ( QWidget* editor, const QModelIndex &index ) const
{
  QString data = index.model()->data ( index ).toString();
  QLineEdit* w = static_cast<QLineEdit*> ( editor );
  w->setText ( data );
}

void CodecTableDelegate::setModelData ( QWidget* editor,
                                        QAbstractItemModel* model,
                                        const QModelIndex &index ) const
{
  QLineEdit* w = static_cast<QLineEdit*> ( editor );
  if ( ! w->text().isEmpty() && index.column() == 0 )
  {
    QRegExp pattern ( "\\-{1,2}[\\w\\d]+" );
    if ( ! w->text().contains ( pattern ) )
    {
      housemaster ( trUtf8 ( "Missing stated hyphens in given argument" ) );
      model->setData ( index, QString(), Qt::EditRole );
      return;
    }
  }
  model->setData ( index, w->text(), Qt::EditRole );
}

void CodecTableDelegate::updateEditorGeometry ( QWidget* editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index ) const
{
  Q_UNUSED ( index );
  editor->setGeometry ( option.rect );
  editor->setContentsMargins ( 0, 0, 0, 0 );
}

CodecTableDelegate::~CodecTableDelegate()
{}

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
#include "optionsfinder.h"
#include "selectvcodecpresets.h"
#include "selectacodecpresets.h"
#include "defaultedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRect>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QItemEditorFactory>

/* QtDBus */
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

CodecTableDelegate::CodecTableDelegate ( QObject * parent )
    : QItemDelegate ( parent )
{
  setObjectName ( QLatin1String ( "CodecTableDelegate" ) );
}

const QString CodecTableDelegate::queryCodec() const
{
  QString codec;
  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  QDBusReply<QString> reply = iface.call ( "editorcodec" );
  if ( reply.isValid() )
    codec = reply.value();

  return codec;
}

const QStringList CodecTableDelegate::hasPredefinedArguments () const
{
  QStringList buffer;
  QString codec = queryCodec();
  if ( ! codec.isEmpty() )
  {
    OptionsFinder* finder = new OptionsFinder ( codec );
    foreach ( QString a, finder->options () )
    {
      buffer.append ( QString::fromUtf8 ( "-%1" ).arg ( a ) );
    }
    delete finder;
  }
  return buffer;
}

const QStringList CodecTableDelegate::hasPredefinedOptions ( const QString &predicate ) const
{
  QStringList buffer;
  if ( predicate.isEmpty() )
    return buffer;

  QString codec = queryCodec();
  if ( ! codec.isEmpty() )
  {
    OptionsFinder* finder = new OptionsFinder ( codec );
    QStringList list = finder->values ( predicate );
    if ( list.size() > 1 )
      buffer.append ( list );

    delete finder;
  }
  return buffer;
}

void CodecTableDelegate::housemaster ( const QString &message ) const
{
  QDBusInterface iface ( "de.hjcms.qx11grab", "/", "de.hjcms.qx11grab" );
  iface.call ( "message", message );
}

const QString CodecTableDelegate::findOption ( const QModelIndex &index ) const
{
  QString option = index.sibling ( index.row(), 0 ).data().toString();
  return option;
}

/**
* Gültige Preset Optionen für sind (vpre, apre und spre).
* Bei -fpre wird ein Pfad zur *.ffpreset verlangt!
*/
CodecTableDelegate::PRESET_TYPE CodecTableDelegate::isPresetsOption ( const QModelIndex &index ) const
{
  QRegExp pattern0 ( "^\\-vpre[:\\w]?" );
  if ( findOption ( index ).contains ( pattern0 ) )
    return VCODEC;

  QRegExp pattern1 ( "^\\-apre[:\\w]?" );
  if ( findOption ( index ).contains ( pattern1 ) )
    return ACODEC;

  return NONE;
}

QWidget* CodecTableDelegate::createEditor ( QWidget* parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index ) const
{
  Q_UNUSED ( option );
  if ( ( index.column() == 1 ) && ( isPresetsOption ( index ) == VCODEC ) )
  {
    SelectVcodecPresets* w = new SelectVcodecPresets ( parent );
    w->setValue ( index.model()->data ( index ) );
    return w;
  }
  else if ( ( index.column() == 1 ) && ( isPresetsOption ( index ) == ACODEC ) )
  {
    SelectAcodecPresets* w = new SelectAcodecPresets ( parent );
    w->setValue ( index.model()->data ( index ) );
    return w;
  }

  DefaultEdit* w = new DefaultEdit ( parent );
  w->setValue ( index.model()->data ( index ) );
  return w;
}

void CodecTableDelegate::setEditorData ( QWidget* editor, const QModelIndex &index ) const
{
  if ( ( index.column() == 1 ) && ( isPresetsOption ( index ) == VCODEC ) )
  {
    SelectVcodecPresets* w = static_cast<SelectVcodecPresets*> ( editor );
    w->setValue ( index.model()->data ( index ) );
    return;
  }
  else if ( ( index.column() == 1 ) && ( isPresetsOption ( index ) == ACODEC ) )
  {
    SelectAcodecPresets* w = static_cast<SelectAcodecPresets*> ( editor );
    w->setValue ( index.model()->data ( index ) );
    return;
  }

  DefaultEdit* w = static_cast<DefaultEdit*> ( editor );
  QStringList opts;
  if ( index.column() == 0 )
    opts = hasPredefinedArguments();
  else if ( index.column() == 1 )
  {
    QString id = findOption ( index );
    if ( ! id.isEmpty() )
    {
      w->setCompleterId ( id );
      opts = hasPredefinedOptions ( id );
    }
  }
  if ( opts.size() > 0 )
    w->setCompleters ( opts );

  w->setValue ( index.model()->data ( index ) );
}

void CodecTableDelegate::setModelData ( QWidget* editor,
                                        QAbstractItemModel* model,
                                        const QModelIndex &index ) const
{
  if ( ( index.column() == 1 ) && ( isPresetsOption ( index ) == VCODEC ) )
  {
    SelectVcodecPresets* w = static_cast<SelectVcodecPresets*> ( editor );
    model->setData ( index, w->value(), Qt::EditRole );
    return;
  }
  else if ( ( index.column() == 1 ) && ( isPresetsOption ( index ) == ACODEC ) )
  {
    SelectAcodecPresets* w = static_cast<SelectAcodecPresets*> ( editor );
    model->setData ( index, w->value(), Qt::EditRole );
    return;
  }

  DefaultEdit* w = static_cast<DefaultEdit*> ( editor );
  if ( ! w->value().toString().isEmpty() && index.column() == 0 )
  {
    QRegExp pattern ( "\\-{1,2}[\\w\\d]+" );
    if ( ! w->text().contains ( pattern ) )
    {
      /*: StatusBar Message */
      housemaster ( trUtf8 ( "Missing stated hyphens in given argument" ) );
      model->setData ( index, QString(), Qt::EditRole );
      return;
    }
  }
  model->setData ( index, w->value(), Qt::EditRole );
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

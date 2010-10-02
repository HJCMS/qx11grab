/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2010
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

#include "tableeditor.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHashIterator>
#include <QtCore/QRect>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QPalette>

/* FFmpeg */
extern "C"
{
#include <libavutil/common.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

TableEditor::TableEditor ( QWidget * parent )
    : QWidget ( parent )
    , sharedVideoCodec ( 0 )
    , sharedAudioCodec ( 0 )
{
  setupUi ( this );
  setBackgroundRole ( QPalette::Window );
  currentType = QString::null;

  codecSelection->setEditable ( true );

  // onUpdate
  connect ( codecSelection, SIGNAL ( currentIndexChanged ( int ) ),
            this, SIGNAL ( postUpdate() ) );
  connect ( table, SIGNAL ( itemChanged ( QTableWidgetItem * ) ),
            this, SIGNAL ( postUpdate() ) );
  // Item Buttons
  connect ( btnAdd, SIGNAL ( clicked() ), this, SLOT ( addTableRow() ) );
  connect ( btnDel, SIGNAL ( clicked() ), this, SLOT ( delTableRow() ) );
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Video Kodierungen und schreibe diese nach @ref codecSelection.
*/
void TableEditor::findVideoCodecs()
{
  codecSelection->clear();
  av_register_all();
  AVCodec *codec;
  int index = 0;
  for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
  {
    if ( ( codec->type == CODEC_TYPE_VIDEO ) && ( codec->encode ) )
    {
      codecSelection->insertItem ( index, codec->name, QVariant ( codec->name ) );
      codecSelection->setItemData ( index, codec->name, Qt::EditRole );
      codecSelection->setItemData ( index, codec->long_name, Qt::ToolTipRole );
      index++;
    }
  }
  // Eigene Codec Definitionen einfügen
  foreach ( QString custom, sharedVideoCodec )
  {
    if ( codecSelection->findData ( custom ) == -1 )
    {
      codecSelection->insertItem ( index, custom, QVariant ( custom ) );
      codecSelection->setItemData ( index, custom, Qt::EditRole );
      codecSelection->setItemData ( index, trUtf8 ( "Customized" ), Qt::ToolTipRole );
    }
  }
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Audio Kodierungen und schreibe diese nach @ref codecSelection.
*/
void TableEditor::findAudioCodecs()
{
  codecSelection->clear();
  av_register_all();
  AVCodec *codec;
  int index = 0;
  for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
  {
    if ( ( codec->type == CODEC_TYPE_AUDIO ) && ( codec->encode ) )
    {
      codecSelection->insertItem ( index, codec->name, QVariant ( codec->name ) );
      codecSelection->setItemData ( index, codec->name, Qt::EditRole );
      codecSelection->setItemData ( index, codec->long_name, Qt::ToolTipRole );
      index++;
    }
  }
  // Eigene Codec Definitionen einfügen
  foreach ( QString custom, sharedAudioCodec )
  {
    if ( codecSelection->findData ( custom ) == -1 )
    {
      codecSelection->insertItem ( index, custom, QVariant ( custom ) );
      codecSelection->setItemData ( index, custom, Qt::EditRole );
      codecSelection->setItemData ( index, trUtf8 ( "Customized" ), Qt::ToolTipRole );
    }
  }
}

/**
* Einen Tabellen Eintrag erstellen.
*/
QTableWidgetItem* TableEditor::createItem ( const QString &data )
{
  QTableWidgetItem* item = new QTableWidgetItem ( QTableWidgetItem::UserType );
  item->setText ( data );
  item->setData ( Qt::ToolTipRole, data );
  item->setData ( Qt::UserRole, data );
  return item;
}

/**
* Liest ein QSettings Gruppen Array aus
*/
const QHash<QString,QVariant> TableEditor::readSection ( const QString &type, QSettings *cfg )
{
  QHash <QString,QVariant> map;
  int size = cfg->beginReadArray ( type );
  if ( size < 1 )
  {
    cfg->endArray(); // Nicht vergessen ;)
    return map;
  }

  for ( int i = 0; i < size; i++ )
  {
    cfg->setArrayIndex ( i );
    map[ cfg->value ( "argument" ).toString() ] =  cfg->value ( "value", "" );
  }
  cfg->endArray();

  return map;
}

/**
* Lese alle Inhalte aus der Tabelle und Schreibe sie in einen Hash
*/
const QHash<QString,QString> TableEditor::tableItems()
{
  QHash<QString,QString> hash;
  for ( int r = 0; r < table->rowCount(); r++ )
  {
    // Argument
    QTableWidgetItem* keyItem = table->item ( r, 0 );
    /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
    * Diese aber nicht Bearbeitet, dann verhindern das
    * an dieser Stelle das Programm wegen fehlenden
    * QTableWidgetItem Pointer nicht abstürzt! */
    if ( ! keyItem || keyItem->text().isEmpty() )
      continue;

    // Wertzuweisung
    QTableWidgetItem* valItem = table->item ( r, 1 );

    if ( valItem && ! valItem->text().isEmpty() )
      hash[keyItem->text() ] = valItem->text();
    else
      hash[keyItem->text() ] = "";
  }
  return hash;
}

/**
* Tabelle befüllen
*/
void TableEditor::loadTableOptions ( const QString &type, QSettings *cfg )
{
  int row = 0;
  QHash<QString,QVariant> map = readSection ( type, cfg );
  if ( map.size() < 1 )
    return;

  table->clearContents();
  table->setRowCount ( map.size() );

  QHashIterator<QString,QVariant> it ( map );
  while ( it.hasNext() )
  {
    it.next();
    table->setItem ( row, 0, createItem ( it.key() ) );
    if ( it.value().toString().isEmpty() )
      table->setItem ( row, 1, createItem ( QString::null ) );
    else
      table->setItem ( row, 1, createItem ( it.value().toString() ) );

    row++;
  }
  addTableRow();
}

/**
* Alle Tabellen Inhalte Speichern
*/
void TableEditor::saveTableOptions ( const QString &type, QSettings *cfg )
{
  QHash<QString,QString> hash = tableItems();
  if ( hash.size() < 1 )
    return;

  int row = 0;
  cfg->remove ( type );
  cfg->beginWriteArray ( type );
  QHashIterator<QString,QString> it ( hash );
  while ( it.hasNext() )
  {
    it.next();
    cfg->setArrayIndex ( row );
    cfg->setValue ( "argument", it.key() );
    cfg->setValue ( "value", it.value() );
    row++;
  }
  cfg->endArray();
}

/**
* Eine neue Zeile ind Tabelle @ref table einfügen.
*/
void TableEditor::addTableRow()
{
  int cur = table->rowCount();
  table->setRowCount ( ++cur );
}

/**
* Ausgewählte Zeile aus @ref table entfernen.
*/
void TableEditor::delTableRow()
{
  QList<QTableWidgetItem *> items = table->selectedItems ();
  if ( items.size() < 1 )
    return;

  int r = items.first()->row();
  if ( r >= 0 )
    table->removeRow ( r );

  emit postUpdate();
}

/**
* Standard Laden
*/
void TableEditor::load ( const QString &type, QSettings *cfg )
{
  int codecIndex = 0;
  currentType = type;
  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    sharedVideoCodec << cfg->value ( "video_codec" ).toString();
    findVideoCodecs();
    codecIndex = codecSelection->findData ( cfg->value ( "video_codec" ) );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    sharedAudioCodec << cfg->value ( "audio_codec" ).toString();
    findAudioCodecs();
    codecIndex = codecSelection->findData ( cfg->value ( "audio_codec" ) );
  }
  codecSelection->setCurrentIndex ( codecIndex );
  loadTableOptions ( type, cfg );
}

/**
* Standard Speichern
* @ref saveTableOptions
*/
void TableEditor::save ( const QString &type, QSettings *cfg )
{
  saveTableOptions ( type, cfg );
  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    cfg->setValue ( QLatin1String ( "video_codec" ),
                    codecSelection->itemText ( codecSelection->currentIndex() ) );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cfg->setValue ( QLatin1String ( "audio_codec" ),
                    codecSelection->itemText ( codecSelection->currentIndex() ) );
  }
}

/**
* Die Komplette Argumenten Liste ausgeben
*/
const QStringList TableEditor::getCmd ()
{
  QStringList cmd;

  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    cmd << QLatin1String ( "-vcodec" );
    cmd << codecSelection->itemText ( codecSelection->currentIndex() );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cmd << QLatin1String ( "-acodec" );
    cmd << codecSelection->itemText ( codecSelection->currentIndex() );
  }

  QHash<QString,QString> hash = tableItems();
  if ( hash.size() >= 1 )
  {
    QHashIterator<QString,QString> it ( hash );
    while ( it.hasNext() )
    {
      it.next();
      cmd << it.key();
      if ( ! it.value().isEmpty() )
        cmd << it.value();
    }
  }
  return cmd;
}

TableEditor::~TableEditor()
{}

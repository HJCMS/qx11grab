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
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QPalette>

/* FFmpeg */
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

TableEditor::TableEditor ( QWidget * parent )
    : QWidget ( parent )
{
  setupUi ( this );
  setBackgroundRole ( QPalette::Window );
  currentType = QString::null;

  connect ( btnAdd, SIGNAL ( clicked() ), this, SLOT ( addTableRow() ) );
  connect ( btnDel, SIGNAL ( clicked() ), this, SLOT ( delTableRow() ) );
}

void TableEditor::findVideoCodecs()
{
  av_register_all();
  AVCodec *codec;
  for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
  {
    if ( ( codec->type == CODEC_TYPE_VIDEO ) && ( codec->encode ) )
    {
      QString itemName = QString ( "%1 (%2)" ).arg ( codec->long_name, codec->name );
      codecSelection->addItem ( itemName, QVariant ( codec->name ) );
    }
  }
}

void TableEditor::findAudioCodecs()
{
  av_register_all();
  AVCodec *codec;
  for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
  {
    if ( ( codec->type == CODEC_TYPE_AUDIO ) && ( codec->encode ) )
    {
      QString itemName = QString ( "%1 (%2)" ).arg ( codec->long_name, codec->name );
      codecSelection->addItem ( itemName, QVariant ( codec->name ) );
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
}

void TableEditor::saveTableOptions ( const QString &type, QSettings *cfg )
{
  int rows = table->rowCount();
  cfg->remove ( type );
  if ( rows >= 1 )
  {
    cfg->beginWriteArray ( type );
    for ( int r = 0; r < rows; r++ )
    {
      // Argument
      QTableWidgetItem* keyItem = table->item ( r, 0 );
      /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
      * Diese aber nicht Bearbeitet, dann verhindern das
      * an dieser Stelle das Programm wegen fehlenden
      * QTableWidgetItem Pointer nicht abstürzt! */
      if ( ! keyItem || keyItem->text().isEmpty() )
        continue;

      cfg->setArrayIndex ( r );
      cfg->setValue ( "argument", keyItem->text() );

      // Wertzuweisung
      QTableWidgetItem* valItem = table->item ( r, 1 );
      if ( valItem && ! valItem->text().isEmpty() )
        cfg->setValue ( "value", valItem->text() );
      else
        cfg->setValue ( "value", "" );
    }
    cfg->endArray();
  }
}

/**
* Eine neue Zeile ind Tabelle @ref audioOptions einfügen.
*/
void TableEditor::addTableRow()
{
  int cur = table->rowCount();
  table->setRowCount ( ++cur );
}

/**
* Ausgewählte Zeile aus @ref audioOptions entfernen.
*/
void TableEditor::delTableRow()
{
  QList<QTableWidgetItem *> items = table->selectedItems ();
  if ( items.size() < 1 )
    return;

  int r = items.first()->row();
  if ( r >= 0 )
    table->removeRow ( r );
}

void TableEditor::load ( const QString &type, QSettings *cfg )
{
  int codecIndex = 0;
  currentType = type;
  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    findVideoCodecs();
    codecIndex = codecSelection->findData ( cfg->value ( "video_codec" ) );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    findAudioCodecs();
    codecIndex = codecSelection->findData ( cfg->value ( "audio_codec" ) );
  }
  codecSelection->setCurrentIndex ( codecIndex );
  loadTableOptions ( type, cfg );
}

void TableEditor::save ( const QString &type, QSettings *cfg )
{
  saveTableOptions ( type, cfg );
  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    cfg->setValue ( QLatin1String ( "video_codec" ),
                    codecSelection->itemData ( codecSelection->currentIndex(), Qt::UserRole ).toString() );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cfg->setValue ( QLatin1String ( "audio_codec" ),
                    codecSelection->itemData ( codecSelection->currentIndex(), Qt::UserRole ).toString() );
  }
}

const QString TableEditor::getCmd ( const QString &opts )
{
  QStringList cmd;
  if ( ! opts.isEmpty() )
    cmd << opts.split ( " " );

  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    cmd << QLatin1String ( "-vcodec" );
    cmd << codecSelection->itemData ( codecSelection->currentIndex(), Qt::UserRole ).toString();
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cmd << QLatin1String ( "-acodec" );
    cmd << codecSelection->itemData ( codecSelection->currentIndex(), Qt::UserRole ).toString();
  }

  int rows = table->rowCount();
  if ( rows >= 1 )
  {
    for ( int r = 0; r < rows; r++ )
    {
      // Argument
      QTableWidgetItem* keyItem = table->item ( r, 0 );
      /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
      * Diese aber nicht Bearbeitet, dann verhindern das
      * an dieser Stelle das Programm wegen fehlenden
      * QTableWidgetItem Pointer nicht abstürzt! */
      if ( ! keyItem || keyItem->text().isEmpty() )
        continue;

      cmd << keyItem->text();

      // Wertzuweisung
      QTableWidgetItem* valItem = table->item ( r, 1 );
      if ( valItem && ! valItem->text().isEmpty() )
        cmd << valItem->text();
    }
  }
  return cmd.join ( " " );
}

TableEditor::~TableEditor()
{}
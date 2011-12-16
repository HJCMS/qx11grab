/**
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2012
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
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QPushButton>

/* FFmpeg */
extern "C"
{
#include <libavutil/common.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

TableEditor::TableEditor ( QWidget * parent )
    : QWidget ( parent )
    , currentType ( QString::null )
    , sharedVideoCodec ( 0 )
    , sharedAudioCodec ( 0 )
{
  setObjectName ( QLatin1String ( "TableEditor" ) );
  setBackgroundRole ( QPalette::Window );

  QGridLayout* layout =  new QGridLayout ( this );

  int grow = 0; // grid Row
  layout->addWidget ( new QLabel ( trUtf8 ( "Codec" ) ),
                      grow, 0, 1, 1, Qt::AlignRight );

  // ComboBox
  m_codecComboBox = new QComboBox ( this );
  m_codecComboBox->setObjectName ( QLatin1String ( "TableEditor/ComboBox" ) );
  m_codecComboBox->setEditable ( true );
  layout->addWidget ( m_codecComboBox, grow++, 1, 1, 1 );

  // Table
  m_tableWidget = new QTableWidget ( this );
  m_tableWidget->setObjectName ( QLatin1String ( "TableEditor/TableWidget" ) );
  m_tableWidget->setColumnCount ( 2 );
  QIcon cIcon = QIcon::fromTheme ( "view-form-table" );
  QTableWidgetItem* th0 = new QTableWidgetItem ( cIcon, trUtf8 ( "Parameter" ), QTableWidgetItem::UserType );
  m_tableWidget->setHorizontalHeaderItem ( 0, th0 );
  QTableWidgetItem* th1 = new QTableWidgetItem ( cIcon, trUtf8 ( "Value" ), QTableWidgetItem::UserType );
  m_tableWidget->setHorizontalHeaderItem ( 1, th1 );
  m_tableWidget->setDragEnabled ( true );
  m_tableWidget->setDragDropOverwriteMode ( false );
  m_tableWidget->setDragDropMode ( QAbstractItemView::DragDrop );
  m_tableWidget->setDefaultDropAction ( Qt::MoveAction );
  m_tableWidget->setAlternatingRowColors ( true );
  m_tableWidget->setSelectionMode ( QAbstractItemView::ExtendedSelection );
  m_tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows );
  m_tableWidget->setGridStyle ( Qt::DashLine );
  m_tableWidget->setWordWrap ( false );
  m_tableWidget->setCornerButtonEnabled ( true );
  m_tableWidget->horizontalHeader()->setCascadingSectionResizes ( true );
  m_tableWidget->horizontalHeader()->setDefaultSectionSize ( 150 );
  m_tableWidget->horizontalHeader()->setMinimumSectionSize ( 150 );
  m_tableWidget->horizontalHeader()->setStretchLastSection ( true );
  layout->addWidget ( m_tableWidget, grow++, 0, 1, 2 );

  QDialogButtonBox* buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  QPushButton* m_add = buttonBox->addButton ( trUtf8 ( "Add" ), QDialogButtonBox::ActionRole );
  m_add->setIcon ( QIcon::fromTheme ( "insert-table" ) );
  QPushButton* m_del = buttonBox->addButton ( trUtf8 ( "Remove" ), QDialogButtonBox::ActionRole );
  m_del->setIcon ( QIcon::fromTheme ( "edit-table-delete-row" ) );
  layout->addWidget ( buttonBox, grow++, 0, 1, 2, Qt::AlignRight );

  setLayout ( layout );

  // onUpdate
  connect ( m_codecComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SIGNAL ( postUpdate() ) );

  connect ( m_tableWidget, SIGNAL ( itemChanged ( QTableWidgetItem * ) ),
            this, SIGNAL ( postUpdate() ) );

  // Item Buttons
  connect ( m_add, SIGNAL ( clicked() ),
            this, SLOT ( addTableRow() ) );

  connect ( m_del, SIGNAL ( clicked() ),
            this, SLOT ( delTableRow() ) );
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Video Kodierungen und schreibe diese nach @ref m_codecComboBox.
*/
void TableEditor::findVideoCodecs()
{
  m_codecComboBox->clear();
  av_register_all();
  AVCodec *codec;
  int index = 0;
  for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
  {
    if ( ( codec->type == AVMEDIA_TYPE_VIDEO ) && ( codec->encode ) )
    {
      m_codecComboBox->insertItem ( index, codec->name, QVariant ( codec->name ) );
      m_codecComboBox->setItemData ( index, codec->name, Qt::EditRole );
      m_codecComboBox->setItemData ( index, codec->long_name, Qt::ToolTipRole );
      index++;
    }
  }
  // Eigene Codec Definitionen einfügen
  foreach ( QString custom, sharedVideoCodec )
  {
    if ( m_codecComboBox->findData ( custom ) == -1 )
    {
      m_codecComboBox->insertItem ( index, custom, QVariant ( custom ) );
      m_codecComboBox->setItemData ( index, custom, Qt::EditRole );
      m_codecComboBox->setItemData ( index, trUtf8 ( "Customized" ), Qt::ToolTipRole );
    }
  }
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Audio Kodierungen und schreibe diese nach @ref m_codecComboBox.
*/
void TableEditor::findAudioCodecs()
{
  m_codecComboBox->clear();
  av_register_all();
  AVCodec *codec;
  int index = 0;
  for ( codec = av_codec_next ( 0 ); codec != NULL; codec = av_codec_next ( codec ) )
  {
    if ( ( codec->type == AVMEDIA_TYPE_AUDIO ) && ( codec->encode ) )
    {
      m_codecComboBox->insertItem ( index, codec->name, QVariant ( codec->name ) );
      m_codecComboBox->setItemData ( index, codec->name, Qt::EditRole );
      m_codecComboBox->setItemData ( index, codec->long_name, Qt::ToolTipRole );
      index++;
    }
  }
  // Eigene Codec Definitionen einfügen
  foreach ( QString custom, sharedAudioCodec )
  {
    if ( m_codecComboBox->findData ( custom ) == -1 )
    {
      m_codecComboBox->insertItem ( index, custom, QVariant ( custom ) );
      m_codecComboBox->setItemData ( index, custom, Qt::EditRole );
      m_codecComboBox->setItemData ( index, trUtf8 ( "Customized" ), Qt::ToolTipRole );
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
  for ( int r = 0; r < m_tableWidget->rowCount(); r++ )
  {
    // Argument
    QTableWidgetItem* keyItem = m_tableWidget->item ( r, 0 );
    /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
    * Diese aber nicht Bearbeitet, dann verhindern das
    * an dieser Stelle dass Programm wegen fehlenden
    * QTableWidgetItem Pointer nicht abstürzt! */
    if ( ! keyItem || keyItem->text().isEmpty() )
      continue;

    // Wertzuweisung
    QTableWidgetItem* valItem = m_tableWidget->item ( r, 1 );

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

  m_tableWidget->clearContents();
  m_tableWidget->setRowCount ( map.size() );

  QHashIterator<QString,QVariant> it ( map );
  while ( it.hasNext() )
  {
    it.next();
    m_tableWidget->setItem ( row, 0, createItem ( it.key() ) );
    if ( it.value().toString().isEmpty() )
      m_tableWidget->setItem ( row, 1, createItem ( QString::null ) );
    else
      m_tableWidget->setItem ( row, 1, createItem ( it.value().toString() ) );

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
* Eine neue Zeile ind Tabelle @ref m_tableWidget einfügen.
*/
void TableEditor::addTableRow()
{
  int cur = m_tableWidget->rowCount();
  m_tableWidget->setRowCount ( ++cur );
}

/**
* Ausgewählte Zeile aus @ref m_tableWidget entfernen.
*/
void TableEditor::delTableRow()
{
  QList<QTableWidgetItem *> items = m_tableWidget->selectedItems ();
  if ( items.size() < 1 )
    return;

  int r = items.first()->row();
  if ( r >= 0 )
    m_tableWidget->removeRow ( r );

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
    codecIndex = m_codecComboBox->findData ( cfg->value ( "video_codec" ) );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    sharedAudioCodec << cfg->value ( "audio_codec" ).toString();
    findAudioCodecs();
    codecIndex = m_codecComboBox->findData ( cfg->value ( "audio_codec" ) );
  }
  m_codecComboBox->setCurrentIndex ( codecIndex );
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
                    m_codecComboBox->itemText ( m_codecComboBox->currentIndex() ) );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cfg->setValue ( QLatin1String ( "audio_codec" ),
                    m_codecComboBox->itemText ( m_codecComboBox->currentIndex() ) );
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
    cmd << m_codecComboBox->itemText ( m_codecComboBox->currentIndex() );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cmd << QLatin1String ( "-acodec" );
    cmd << m_codecComboBox->itemText ( m_codecComboBox->currentIndex() );
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

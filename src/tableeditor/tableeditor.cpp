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
#include "avoptions.h"
#include "codectable.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHashIterator>
#include <QtCore/QRect>
#include <QtCore/QRegExp>
#include <QtCore/QTimer>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QPalette>
#include <QtGui/QPushButton>

TableEditor::TableEditor ( QWidget * parent )
    : QWidget ( parent )
    , currentType ( QString::null )
    , sharedVideoCodec ( 0 )
    , sharedAudioCodec ( 0 )
{
  setObjectName ( QLatin1String ( "TableEditor" ) );

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
  m_tableWidget = new CodecTable ( this );
  layout->addWidget ( m_tableWidget, grow++, 0, 1, 2 );

  QDialogButtonBox* buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  QPushButton* m_add = buttonBox->addButton ( trUtf8 ( "Add" ), QDialogButtonBox::ActionRole );
  m_add->setIcon ( QIcon::fromTheme ( "insert-table" ) );
  QPushButton* m_del = buttonBox->addButton ( trUtf8 ( "Remove" ), QDialogButtonBox::ActionRole );
  m_del->setIcon ( QIcon::fromTheme ( "edit-table-delete-row" ) );
  QPushButton* m_clear = buttonBox->addButton ( trUtf8 ( "Clear" ), QDialogButtonBox::ActionRole );
  m_clear->setIcon ( QIcon::fromTheme ( "edit-clear" ) );
  layout->addWidget ( buttonBox, grow++, 0, 1, 2, Qt::AlignRight );

  setLayout ( layout );

  // onUpdate
  connect ( m_codecComboBox, SIGNAL ( currentIndexChanged ( int ) ),
            this, SIGNAL ( postUpdate() ) );

  connect ( m_tableWidget, SIGNAL ( itemChanged () ),
            this, SIGNAL ( postUpdate() ) );

  // Item Buttons
  connect ( m_add, SIGNAL ( clicked() ),
            this, SLOT ( addTableRow() ) );

  connect ( m_del, SIGNAL ( clicked() ),
            this, SLOT ( delTableRow() ) );

  connect ( m_clear, SIGNAL ( clicked () ),
            m_tableWidget, SLOT ( clearContents() ) );
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Video Kodierungen und schreibe diese nach @ref m_codecComboBox.
*/
void TableEditor::findVideoCodecs()
{
  m_codecComboBox->clear();
  int index = 0;
  QList<QX11Options::FFCodec> list = QX11Options::AVOptions::videoCodecs();
  for ( int i = 0; i < list.size(); ++i )
  {
    QX11Options::FFCodec codec = list.at ( i );
    m_codecComboBox->insertItem ( index, codec.name, QVariant ( codec.name ) );
    m_codecComboBox->setItemData ( index, codec.name, Qt::EditRole );
    m_codecComboBox->setItemData ( index, codec.fullname, Qt::ToolTipRole );
    index = i;
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
  int index = 0;
  QList<QX11Options::FFCodec> list = QX11Options::AVOptions::audioCodecs();
  for ( int i = 0; i < list.size(); ++i )
  {
    QX11Options::FFCodec codec = list.at ( i );
    m_codecComboBox->insertItem ( index, codec.name, QVariant ( codec.name ) );
    m_codecComboBox->setItemData ( index, codec.name, Qt::EditRole );
    m_codecComboBox->setItemData ( index, codec.fullname, Qt::ToolTipRole );
    index = i;
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
* Tabelle befüllen
*/
void TableEditor::loadTableOptions ( const QString &type, QSettings *cfg )
{
  QHash<QString,QVariant> map = readSection ( type, cfg );
  setCodecOptions ( map );
}

/**
* Alle Tabellen Inhalte Speichern
*/
void TableEditor::saveTableOptions ( const QString &type, QSettings *cfg )
{
  QHash<QString,QVariant> hash = getTableItems();
  if ( hash.size() < 1 )
    return;

  int row = 0;
  cfg->remove ( type );
  cfg->beginWriteArray ( type );
  QHashIterator<QString,QVariant> it ( hash );
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
  int row = m_tableWidget->rowCount();
  m_tableWidget->insertRow ( ++row );
}

/**
* Ausgewählte Zeile aus @ref m_tableWidget entfernen.
*/
void TableEditor::delTableRow()
{
  foreach ( int r, m_tableWidget->selectedRows () )
  {
    m_tableWidget->removeRow ( r );
  }
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
* Lese alle Inhalte aus der Tabelle und Schreibe sie in einen Hash
*/
const QHash<QString,QVariant> TableEditor::getTableItems()
{
  QHash<QString,QVariant> hash;
  for ( int r = 0; r < m_tableWidget->rowCount(); r++ )
  {
    // Argument
    QPair<QString,QVariant> p = m_tableWidget->item ( r );
    /* NOTICE Wenn der Benutzer eine Leere Zeile einfügt.
    * Diese aber nicht Bearbeitet, dann verhindern das
    * an dieser Stelle dass Programm wegen fehlenden
    * QTableWidgetItem Pointer nicht abstürzt! */
    if ( p.first.isEmpty() )
      continue;

    hash[p.first] = p.second.toString();
  }
  return hash;
}

/**
* Den Aktuell ausgewählten Codec
*/
const QString TableEditor::selectedCodec()
{
  return m_codecComboBox->itemText ( m_codecComboBox->currentIndex() );
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
    cmd << selectedCodec();
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cmd << QLatin1String ( "-acodec" );
    cmd << m_codecComboBox->itemText ( m_codecComboBox->currentIndex() );
  }

  QHash<QString,QVariant> hash = getTableItems();
  if ( hash.size() >= 1 )
  {
    QHashIterator<QString,QVariant> it ( hash );
    while ( it.hasNext() )
    {
      it.next();
      cmd << it.key();
      if ( ! it.value().toString().isEmpty() )
        cmd << it.value().toString();
    }
  }
  return cmd;
}

void TableEditor::setCodecByName ( const QString &txt )
{
  // qDebug() << Q_FUNC_INFO << txt;
  m_codecComboBox->setCurrentIndex ( m_codecComboBox->findData ( txt ) );
}

void TableEditor::setCodecOptions ( const QHash<QString,QVariant> &options )
{
  int row = 0;
  if ( options.size() < 1 )
    return;

  m_tableWidget->clearContents();
  QHashIterator<QString,QVariant> it ( options );
  while ( it.hasNext() )
  {
    it.next();
    m_tableWidget->insertItem ( row, it.key(), it.value() );
    row++;
  }
}

TableEditor::~TableEditor()
{}

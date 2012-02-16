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
#include "settings.h"
#include "avoptions.h"
#include "codectable.h"
#include "selectextension.h"
#include "codecselecter.h"

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
  setContentsMargins ( 5, 5, 5, 5 );

  QGridLayout* layout =  new QGridLayout ( this );

  int grow = 0; // grid Row
  layout->addWidget ( new QLabel ( trUtf8 ( "Encoder" ) ),
                      grow, 0, 1, 1, Qt::AlignRight );

  // ComboBox Codec
  m_codecSelecter = new CodecSelecter ( this );
  /*: WhatsThis */
  m_codecSelecter->setWhatsThis ( trUtf8 ( "codec selecter" ) );
  layout->addWidget ( m_codecSelecter, grow, 1, 1, 1 );

  // ComboBox Extension
  m_selectExtension = new SelectExtension ( this );
  m_selectExtension->setVisible ( false );
  layout->addWidget ( m_selectExtension, grow++, 2, 1, 1 );

  // Table
  m_tableWidget = new CodecTable ( this );
  /*: WhatsThis */
  m_tableWidget->setWhatsThis ( trUtf8 ( "codec editor table" ) );
  layout->addWidget ( m_tableWidget, grow++, 0, 1, 3 );

  QDialogButtonBox* buttonBox = new QDialogButtonBox ( Qt::Horizontal, this );
  QPushButton* m_add = buttonBox->addButton ( trUtf8 ( "Add" ), QDialogButtonBox::ActionRole );
  m_add->setIcon ( Settings::themeIcon ( "insert-table" ) );
  /*: WhatsThis */
  m_add->setWhatsThis ( trUtf8 ( "insert a new empty table row" ) );

  QPushButton* m_del = buttonBox->addButton ( trUtf8 ( "Remove" ), QDialogButtonBox::ActionRole );
  m_del->setIcon ( Settings::themeIcon ( "edit-table-delete-row" ) );
  /*: WhatsThis */
  m_del->setWhatsThis ( trUtf8 ( " remove selected table rows" ) );

  QPushButton* m_clear = buttonBox->addButton ( trUtf8 ( "Clear" ), QDialogButtonBox::ActionRole );
  m_clear->setIcon ( Settings::themeIcon ( "edit-clear" ) );
  /*: WhatsThis */
  m_clear->setWhatsThis ( trUtf8 ( "clear table contents" ) );

  layout->addWidget ( buttonBox, grow++, 0, 1, 3, Qt::AlignRight );

  setLayout ( layout );

  // onUpdate
  connect ( m_codecSelecter, SIGNAL ( currentIndexChanged ( int ) ), this, SIGNAL ( postUpdate() ) );
  connect ( m_selectExtension, SIGNAL ( currentIndexChanged ( int ) ), this, SIGNAL ( postUpdate() ) );
  connect ( m_codecSelecter, SIGNAL ( currentIndexChanged ( const QString & ) ),
            m_selectExtension, SLOT ( insertItems ( const QString & ) ) );

  // table context signals
  connect ( m_tableWidget, SIGNAL ( postUpdate() ), this, SIGNAL ( postUpdate() ) );
  connect ( m_tableWidget, SIGNAL ( insertEmptyRow() ), this, SLOT ( addTableRow() ) );
  connect ( m_tableWidget, SIGNAL ( removeSelectedRow() ), this, SLOT ( delTableRow() ) );

  // Item Buttons
  connect ( m_add, SIGNAL ( clicked() ), this, SLOT ( addTableRow() ) );
  connect ( m_del, SIGNAL ( clicked() ), this, SLOT ( delTableRow() ) );
  connect ( m_clear, SIGNAL ( clicked() ), m_tableWidget, SLOT ( clearContents() ) );
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Video Kodierungen und schreibe diese nach @ref m_codecSelecter.
*/
void TableEditor::findVideoCodecs()
{
  QList<QX11Grab::FFCodec> list = QX11Grab::AVOptions::videoCodecs();
  if ( list.size() > 0 )
    m_codecSelecter->setCodecItems ( list );

  // Eigene Codec Definitionen einfügen
  foreach ( QString custom, sharedVideoCodec )
  {
    if ( m_codecSelecter->findData ( custom ) == -1 )
      m_codecSelecter->setCustomItem ( custom, QVariant ( custom ) );
  }
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Audio Kodierungen und schreibe diese nach @ref m_codecSelecter.
*/
void TableEditor::findAudioCodecs()
{
  QList<QX11Grab::FFCodec> list = QX11Grab::AVOptions::audioCodecs();
  if ( list.size() > 0 )
    m_codecSelecter->setCodecItems ( list );

  // Eigene Codec Definitionen einfügen
  foreach ( QString custom, sharedAudioCodec )
  {
    if ( m_codecSelecter->findData ( custom ) == -1 )
      m_codecSelecter->setCustomItem ( custom, QVariant ( custom ) );
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
  foreach ( int r, m_tableWidget->selectedRows() )
  {
    m_tableWidget->removeRow ( r );
  }
}

/**
* Standard Laden
*/
void TableEditor::load ( const QString &type, QSettings *cfg )
{
  currentType = type;
  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    QString vcodec = cfg->value ( "video_codec" ).toString();
    sharedVideoCodec << vcodec;
    findVideoCodecs();
    m_selectExtension->setVisible ( true );
    m_selectExtension->setEnabled ( true );
    m_codecSelecter->setCodec ( vcodec );
    m_selectExtension->setDefault ( cfg->value ( "video_codec_extension" ).toString() );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    sharedAudioCodec << cfg->value ( "audio_codec" ).toString();
    findAudioCodecs();
    m_codecSelecter->setCodec ( cfg->value ( "audio_codec" ).toString() );
  }
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
    cfg->setValue ( QLatin1String ( "video_codec" ), selectedCodec() );
    cfg->setValue ( QLatin1String ( "video_codec_extension" ), m_selectExtension->extension() );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
    cfg->setValue ( QLatin1String ( "audio_codec" ), selectedCodec() );
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
  return m_codecSelecter->getCodec();
}

/**
* Gibt die Aktuelle Erweiterung für diesen Codec zurück
*/
const QString TableEditor::selectedCodecExtension()
{
  return m_selectExtension->extension();
}

/**
* Die Komplette Argumenten Liste ausgeben
*/
const QStringList TableEditor::getCmd()
{
  QStringList cmd;

  if ( currentType.contains ( QLatin1String ( "VideoOptions" ) ) )
  {
    cmd << QLatin1String ( "-vcodec" ) << selectedCodec();
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    cmd << QLatin1String ( "-acodec" ) << selectedCodec();
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
  m_codecSelecter->setCodec ( txt );
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

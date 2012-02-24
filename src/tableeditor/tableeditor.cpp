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
#include "formatmenu.h"
#include "codecselecter.h"
#include "optionsfinder.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QHashIterator>
#include <QtCore/QRect>
#include <QtCore/QRegExp>
#include <QtCore/QTimer>

/* QtGui */
#include <QtGui/QAbstractItemView>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QIcon>
#include <QtGui/QPalette>
#include <QtGui/QToolBar>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

TableEditor::TableEditor ( QWidget * parent )
    : QWidget ( parent )
    , currentType ( QString::null )
    , sharedVideoCodec ( 0 )
    , sharedAudioCodec ( 0 )
    , currentCodecExtension ( QString ( "avi" ) )
{
  setObjectName ( QLatin1String ( "TableEditor" ) );
  setContentsMargins ( 5, 5, 5, 5 );

  QVBoxLayout* layout =  new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  QToolBar* m_toolBar =  new QToolBar ( this );
  m_toolBar->addWidget ( new QLabel ( trUtf8 ( "Encoder" ) ) );

  // ComboBox Codec
  m_codecSelecter = new QX11Grab::CodecSelecter ( this );
  /*: WhatsThis */
  m_codecSelecter->setWhatsThis ( trUtf8 ( "codec selecter" ) );
  m_toolBar->addWidget ( m_codecSelecter );

  // ComboBox Extension
  m_formatMenu = new QX11Grab::FormatMenu ( this );
  m_toolBar->addWidget ( m_formatMenu );

  layout->addWidget ( m_toolBar );

  // Table
  m_tableWidget = new CodecTable ( this );
  /*: WhatsThis */
  m_tableWidget->setWhatsThis ( trUtf8 ( "codec editor table" ) );
  layout->addWidget ( m_tableWidget );

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

  layout->addWidget ( buttonBox, Qt::AlignRight );

  setLayout ( layout );

  // ComboBoxes
  connect ( m_codecSelecter, SIGNAL ( codecChanged ( const QString &, CodecID ) ),
            m_formatMenu, SLOT ( updateMenu ( const QString &, CodecID ) ) );

  connect ( m_formatMenu, SIGNAL ( extensionChanged ( const QString & ) ),
            this, SLOT ( setCodecExtension ( const QString & ) ) );

  connect ( m_formatMenu, SIGNAL ( postUpdate() ), this, SIGNAL ( postUpdate() ) );

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
* Wenn im Menü Erweiterungen eine Eintrag ausgewählt wird!
*/
void TableEditor::setCodecExtension ( const QString &ext )
{
  currentCodecExtension = ext;
  emit postUpdate();
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
    currentCodecExtension = cfg->value ( "video_codec_extension" ).toString();
    m_formatMenu->setEnabled ( true );
    sharedVideoCodec << vcodec;
    findVideoCodecs();
    m_codecSelecter->setCodec ( vcodec );
    m_formatMenu->setEntryEnabled ( currentCodecExtension );
  }
  else if ( currentType.contains ( QLatin1String ( "AudioOptions" ) ) )
  {
    m_formatMenu->setEnabled ( false );
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
    cfg->setValue ( QLatin1String ( "video_codec_extension" ), currentCodecExtension );
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
    * QTableWidgetItem Pointer abstürzt! */
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
  // Wenn Leer dann bei XML OptionsFinder suchen!
  if ( currentCodecExtension.isEmpty() )
  {
    QX11Grab::OptionsFinder finder ( selectedCodec() );
    QList<QX11Grab::VideoExtension> list = finder.extensionList();
    for ( int i = 0; i < list.size(); ++i )
    {
      if ( list.at ( i ).isDefault )
      {
        QString entry = list.at ( i ).name;
        m_formatMenu->setEntryEnabled ( entry );
        currentCodecExtension = entry;
        break;
      }
    }
  }
  return currentCodecExtension;
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

/**
* Setzt den Kodierer mit Namen, wird Üblicherweise von
* Lesezeichen aus dem Hauptfenster aufgerufen!
*/
void TableEditor::setCodecByName ( const QString &txt )
{
  currentCodecExtension.clear();
  m_codecSelecter->setCodec ( txt );
}

/**
* Setzt den Kodierer Optionen, wird Üblicherweise von
* Lesezeichen aus dem Hauptfenster aufgerufen!
*/
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

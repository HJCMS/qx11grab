/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann http://qx11grab.hjcms.de, (C) 2007-2015
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
*/

#include "abstracttableeditor.h"

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

AbstractTableEditor::AbstractTableEditor ( QWidget * parent, bool extmenu )
    : QWidget ( parent )
    , enableExtMenu ( extmenu )
{
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
  if ( enableExtMenu )
  {
    m_formatMenu = new QX11Grab::FormatMenu ( this );
    m_toolBar->addWidget ( m_formatMenu );
  }

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
* Liest ein QSettings Gruppen Array aus
*/
const QHash<QString,QVariant> AbstractTableEditor::readSection ( const QString &type, Settings *cfg )
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
void AbstractTableEditor::loadTableOptions ( const QString &type, Settings *cfg )
{
  QHash<QString,QVariant> map = readSection ( type, cfg );
  setCodecOptions ( map );
}

/**
* Alle Tabellen Inhalte Speichern
*/
void AbstractTableEditor::saveTableOptions ( const QString &type, Settings *cfg )
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
void AbstractTableEditor::addTableRow()
{
  int row = m_tableWidget->rowCount();
  m_tableWidget->insertRow ( ++row );
}

/**
* Ausgewählte Zeile aus @ref m_tableWidget entfernen.
*/
void AbstractTableEditor::delTableRow()
{
  foreach ( int r, m_tableWidget->selectedRows() )
  {
    m_tableWidget->removeRow ( r );
  }
}

/**
* Lese alle Inhalte aus der Tabelle und Schreibe sie in einen Hash
*/
const QHash<QString,QVariant> AbstractTableEditor::getTableItems()
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
const QString AbstractTableEditor::selectedCodec()
{
  return m_codecSelecter->getCodec();
}

/**
* Setzt den Kodierer mit Namen, wird Üblicherweise von
* Lesezeichen aus dem Hauptfenster aufgerufen!
*/
void AbstractTableEditor::setCodecByName ( const QString &txt )
{
  m_codecSelecter->setCodec ( txt );
}

/**
* Setzt den Kodierer Optionen, wird Üblicherweise von
* Lesezeichen aus dem Hauptfenster aufgerufen!
*/
void AbstractTableEditor::setCodecOptions ( const QHash<QString,QVariant> &options )
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

AbstractTableEditor::~AbstractTableEditor()
{}

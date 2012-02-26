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

#include "audiotableeditor.h"

/* QtCore */
#include <QtCore/QDebug>

AudioTableEditor::AudioTableEditor ( QWidget * parent )
    : AbstractTableEditor ( parent, false )
    , sharedAudioCodec ( 0 )
{
  setObjectName ( QLatin1String ( "acodec" ) );
  setToolTip ( QString::fromUtf8 ( "-acodec" ) );
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Audio Kodierungen und schreibe diese nach @ref m_codecSelecter.
*/
void AudioTableEditor::findAudioCodecs()
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
* Standard Laden
*/
void AudioTableEditor::load ( Settings * cfg )
{
  sharedAudioCodec << cfg->value ( "audio_codec" ).toString();
  findAudioCodecs();
  m_codecSelecter->setCodec ( cfg->value ( "audio_codec" ).toString() );
  loadTableOptions ( QString::fromUtf8 ( "AudioOptions" ), cfg );
}

/**
* Standard Speichern
* @ref saveTableOptions
*/
void AudioTableEditor::save ( Settings *cfg )
{
  saveTableOptions ( QString::fromUtf8 ( "AudioOptions" ), cfg );
  cfg->setValue ( QLatin1String ( "audio_codec" ), selectedCodec() );
}

/**
* Die Komplette Argumenten Liste ausgeben
*/
const QStringList AudioTableEditor::getCmd()
{
  QStringList cmd;
  cmd << QLatin1String ( "-acodec" ) << selectedCodec();

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

AudioTableEditor::~AudioTableEditor()
{}

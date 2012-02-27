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

#include "videotableeditor.h"

/* QtCore */
#include <QtCore/QDebug>

VideoTableEditor::VideoTableEditor ( QWidget * parent )
    : AbstractTableEditor ( parent, true )
    , sharedVideoCodec ( 0 )
    , currentCodecExtension ( QString ( "avi" ) )
{
  setObjectName ( QLatin1String ( "vcodec" ) );
  setToolTip ( QString::fromUtf8 ( "-vcodec" ) );

  // ComboBoxes
  connect ( m_codecSelecter, SIGNAL ( codecChanged ( const QString &, CodecID ) ),
            m_formatMenu, SLOT ( updateMenu ( const QString &, CodecID ) ) );

  connect ( m_formatMenu, SIGNAL ( extensionChanged ( const QString & ) ),
            this, SLOT ( setCodecExtension ( const QString & ) ) );

  connect ( m_formatMenu, SIGNAL ( postUpdate() ), this, SLOT ( formatUpdated() ) );
}

/**
* Such mit Hilfe von AVCodec alle verfügbaren
* Video Kodierungen und schreibe diese nach @ref m_codecSelecter.
*/
void VideoTableEditor::findVideoCodecs()
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
* Wenn im Menü Erweiterungen eine Eintrag ausgewählt wird!
*/
void VideoTableEditor::formatUpdated()
{
  currentCodecExtension = m_formatMenu->currentExtension();
  emit postUpdate();
}

/**
* Wenn im Menü Erweiterungen eine Eintrag ausgewählt wird!
*/
void VideoTableEditor::setCodecExtension ( const QString &ext )
{
  // qDebug() << Q_FUNC_INFO << ext;
  currentCodecExtension = ext;
  emit postUpdate();
}

/**
* Standard Laden
*/
void VideoTableEditor::load ( Settings *cfg )
{
  QString vcodec = cfg->value ( "video_codec" ).toString();
  // Suche mit dem Codec nach der passenden erweiterung
  currentCodecExtension = cfg->getArrayItem ( "CodecExtensions", "format", vcodec, "defaultExt" ).toString();
  if ( currentCodecExtension.isEmpty() )
    currentCodecExtension =  cfg->value ( "video_codec_extension" ).toString();

  m_formatMenu->setEnabled ( true );
  sharedVideoCodec << vcodec;
  findVideoCodecs();
  m_codecSelecter->setCodec ( vcodec );
  m_formatMenu->setEntryEnabled ( currentCodecExtension );
  loadTableOptions ( QString::fromUtf8 ( "VideoOptions" ), cfg );
}

/**
* Standard Speichern
* @ref saveTableOptions
*/
void VideoTableEditor::save ( Settings *cfg )
{
  saveTableOptions ( QString::fromUtf8 ( "VideoOptions" ), cfg );
  cfg->setValue ( QLatin1String ( "video_codec" ), selectedCodec() );
  cfg->setValue ( QLatin1String ( "video_codec_extension" ), currentCodecExtension );
}

/**
* Gibt die Aktuelle Erweiterung für diesen Codec zurück
*/
const QString VideoTableEditor::selectedCodecExtension()
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
const QStringList VideoTableEditor::getCmd()
{
  QStringList cmd;
  cmd << QLatin1String ( "-vcodec" ) << selectedCodec();

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

VideoTableEditor::~VideoTableEditor()
{}

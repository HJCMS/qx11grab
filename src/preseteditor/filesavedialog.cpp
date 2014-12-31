/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#include "filesavedialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

static const QString ffmpegHome()
{
  QString home = QDir::homePath();
  QDir dir ( home + "/.ffmpeg/" );
  if ( ! dir.exists () )
    dir.mkpath ( home + "/.ffmpeg/" );

  return dir.absolutePath();
}

FileSaveDialog::FileSaveDialog ( const QString &name, const QString &content, QWidget * parent )
    : QFileDialog ( parent, QString(), ffmpegHome() )
    , fileName ( QString::fromUtf8 ( "%1_QX11Grab.ffpreset" ).arg ( name ) )
    , fileContent ( content )
{
  setObjectName ( QLatin1String ( "FileSaveDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Save ffpreset File" ) );
  /*: ToolTip */
  setNameFilter ( trUtf8 ( "FFmpeg preset file (*.ffpreset)" ) );
  setDefaultSuffix ( QLatin1String ( "ffpreset" ) );
  setAcceptMode ( QFileDialog::AcceptSave );
  setOptions ( ( QFileDialog::ShowDirsOnly | QFileDialog::HideNameFilterDetails ) );
  setFileMode ( QFileDialog::AnyFile );
  setViewMode ( QFileDialog::Detail );
  setDirectory ( ffmpegHome() );
  selectFile ( fileName );

  connect ( this, SIGNAL ( accepted () ), this, SLOT ( saveAndExit() ) );
}

void FileSaveDialog::saveAndExit()
{
  QString out = selectedFiles().last();
  QFileInfo info ( out );
  if ( info.isDir() )
    info.setFile ( fileName );

  QFile fp ( info.absoluteFilePath() );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << fileContent;
    fp.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner ) );
    fp.close();
  }
  emit finished ( 1 );
}

FileSaveDialog::~FileSaveDialog()
{}

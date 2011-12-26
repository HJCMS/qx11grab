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

#include "exportdialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

/* QtGui */
#include <QtGui/QDesktopServices>

ExportDialog::ExportDialog ( const QString &command, QWidget * parent )
    : QFileDialog ( parent )
    , line ( command )
{
  setObjectName ( QLatin1String ( "ExportDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Export current Commandline" ) );

  QString dest = QDesktopServices::storageLocation ( QDesktopServices::DocumentsLocation );
  setAcceptMode ( QFileDialog::AcceptSave );
  setOptions ( QFileDialog::ShowDirsOnly );
  setFileMode ( QFileDialog::AnyFile );
  setViewMode ( QFileDialog::Detail );
  setDirectory ( dest );
  setNameFilter ( trUtf8 ( "Shell Script (*.sh)" ) );
  setDefaultSuffix ( QLatin1String ( "sh" ) );
  selectFile ( QLatin1String ( "qx11grab.sh" ) );

  connect ( this, SIGNAL ( accepted () ), this, SLOT ( save() ) );
}

void ExportDialog::save()
{
  QString out = selectedFiles().last();
  QFileInfo info ( out );
  if ( info.isDir() )
    info.setFile ( "qx11grab.sh" );

  QStringList cmd ( "#!/bin/bash" );
  cmd << "" << "# command line export from QX11Grab Application http://qx11grab.hjcms.de";
  cmd << "" << line << "";
  QFile fp ( info.absoluteFilePath() );
  if ( fp.open ( QIODevice::WriteOnly ) )
  {
    QTextStream stream ( &fp );
    stream << cmd.join ( "\n" );
    fp.setPermissions ( ( QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner ) );
    fp.close();
  }
  emit finished ( 1 );
}

ExportDialog::~ExportDialog()
{}

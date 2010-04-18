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

#include "defaults.h"

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>

Defaults::Defaults ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "defaults" ) );

  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 2, 2, 2, 2 );
  gridLayout->setObjectName ( QLatin1String ( "gridLayout" ) );

  QIcon folderIcon;
  folderIcon.addFile ( QString::fromUtf8 ( ":/images/folder_normal.png" ), QSize(), QIcon::Normal, QIcon::Off );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  // begin: FFmpeg Binary
  QLabel* txt_ffbinary = new QLabel ( this );
  txt_ffbinary->setText ( trUtf8 ( "FFmpeg Application:" ) );
  txt_ffbinary->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_ffbinary, 0, 0, 1, 1 );

  ff_path = new QLineEdit ( this );
  ff_path->setObjectName ( QLatin1String ( "ff_path" ) );
  ff_path->setText ( QString::fromUtf8 ( "/usr/bin/ffmpeg" ) );
  gridLayout->addWidget ( ff_path, 0, 1, 1, 1 );

  QToolButton* setFFmpegBtn = new QToolButton ( this );
  setFFmpegBtn->setIcon ( folderIcon );
  gridLayout->addWidget ( setFFmpegBtn, 0, 2, 1, 1 );
  // end: FFmpeg Binary

  // begin: Output Filename
  QLabel* txt_outfile = new QLabel ( this );
  txt_outfile->setText ( trUtf8 ( "Output Filename:" ) );
  txt_outfile->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_outfile, 1, 0, 1, 1 );

  outputName = new QLineEdit ( this );
  outputName->setObjectName ( QLatin1String ( "outputName" ) );
  outputName->setText ( QString::fromUtf8 ( "qx11grab-XXXXXX.avi" ) );
  gridLayout->addWidget ( outputName, 1, 1, 1, 2 );
  // end: Output Filename

  // begin: Output Directory
  QLabel* txt_outdir = new QLabel ( this );
  txt_outdir->setText ( trUtf8 ( "Output Directory:" ) );
  txt_outdir->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_outdir, 2, 0, 1, 1 );

  outputDirectory = new QLineEdit ( this );
  outputDirectory->setObjectName ( QLatin1String ( "tempdir" ) );
  outputDirectory->setText ( QString::fromUtf8 ( "/tmp" ) );
  gridLayout->addWidget ( outputDirectory, 2, 1, 1, 1 );

  QToolButton* setOutputBtn = new QToolButton ( this );
  setOutputBtn->setIcon ( folderIcon );
  gridLayout->addWidget ( setOutputBtn, 2, 2, 1, 1 );
  // end: Output Directory

  QSpacerItem* spacer  = new QSpacerItem ( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
  gridLayout->addItem ( spacer, 3, 0, 1, 2 );

  setLayout ( gridLayout );

  connect ( setFFmpegBtn, SIGNAL ( clicked() ), this, SLOT ( setFFmpegBinary() ) );
  connect ( setOutputBtn, SIGNAL ( clicked() ), this, SLOT ( setOutpuDirectory() ) );
}

/**
* Setze den Pfad zum FFmpeg Programm
*/
void Defaults::setFFmpegBinary()
{
  QString path ( "/usr/bin" );
  QStringList filt;
  filt << trUtf8 ( "FFmpeg %1" ).arg ( "ffmpeg* *ffmpeg" );
  filt << trUtf8 ( "Other %1" ).arg ( "*" );

  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "FFmpeg Binary" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
    ff_path->setText ( db.absoluteFilePath() );
}

/**
* Setze den Pfad zum Ausgabe Verzeichnis
*/
void Defaults::setOutpuDirectory()
{
  QString path ( "/tmp" );
  path = QFileDialog::getExistingDirectory ( this, trUtf8 ( "Output Directory" ), path );

  QFileInfo db ( path );
  if ( db.isWritable() )
    outputDirectory->setText ( path );
}

/**
* Einstellungen Laden
*/
void Defaults::load ( QSettings * cfg )
{
  QList<QLineEdit*> items;
  items << outputDirectory << outputName << ff_path;

  foreach ( QLineEdit* edit, items )
  {
    edit->setText ( cfg->value ( edit->objectName() ).toString() );
  }
}

/**
* Einstellungen Speichern
*/
void Defaults::save ( QSettings * cfg )
{
  QList<QLineEdit*> items;
  items << outputDirectory << outputName << ff_path;

  foreach ( QLineEdit* edit, items )
  {
    if ( edit->text().isEmpty() )
      cfg->remove ( edit->objectName() );
    else
      cfg->setValue ( edit->objectName(), edit->text() );
  }
}

Defaults::~Defaults()
{}

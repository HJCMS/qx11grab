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

#include "targetswidget.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>

// #include <QtGui/>

TargetsWidget::TargetsWidget ( QWidget * parent )
    : QGroupBox ( parent )
{
  setObjectName ( QLatin1String ( "TargetsWidget" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "Directories && Files" ) );
  setFlat ( true );

  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 2, 2, 2, 2 );
  gridLayout->setObjectName ( QLatin1String ( "TargetsWidget/GridLayout" ) );

  QIcon folderIcon = QIcon::fromTheme ( "folder-open" );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  int grow = 0;
  // begin: FFmpeg Binary
  QLabel* txt_ffbinary = new QLabel ( this );
  txt_ffbinary->setText ( trUtf8 ( "FFmpeg Application:" ) );
  txt_ffbinary->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_ffbinary, grow, 0, 1, 1 );

  m_binaryFile = new QLineEdit ( this );
  m_binaryFile->setObjectName ( QLatin1String ( "Targets/Binary" ) );
  m_binaryFile->setText ( QLatin1String ( "/usr/bin/ffmpeg" ) );
  /*: WhatsThis */
  m_binaryFile->setWhatsThis ( trUtf8 ( "absolute path to FFmpeg's recording application" ) );
  gridLayout->addWidget ( m_binaryFile, grow, 1, 1, 1 );

  QToolButton* setFFmpegBtn = new QToolButton ( this );
  setFFmpegBtn->setIcon ( folderIcon );
  gridLayout->addWidget ( setFFmpegBtn, grow++, 2, 1, 1 );
  // end: FFmpeg Binary

  // begin: Output Filename
  QLabel* txt_outfile = new QLabel ( this );
  txt_outfile->setText ( trUtf8 ( "Output Filename:" ) );
  txt_outfile->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_outfile, grow, 0, 1, 1 );

  m_outputName = new QLineEdit ( this );
  m_outputName->setObjectName ( QLatin1String ( "Targets/TemplateName" ) );
  m_outputName->setText ( QString::fromUtf8 ( "qx11grab-XXXXXX" ) );
  /*: WhatsThis */
  m_outputName->setWhatsThis ( trUtf8 ( "Specified output filenames can contain the following template XXXXXX (six upper case \"X\" characters), which will be replaced by the auto-generated portion of the filename." ) );
  gridLayout->addWidget ( m_outputName, grow, 1, 1, 1 );

  QToolButton* restoreButton = new QToolButton ( this );
  restoreButton->setIcon ( QIcon::fromTheme ( "edit-clear-locationbar-rtl" ) );
  gridLayout->addWidget ( restoreButton, grow++, 2, 1, 1 );
  // end: Output Filename

  // begin: Output Directory
  QLabel* txt_outdir = new QLabel ( this );
  txt_outdir->setText ( trUtf8 ( "Output Directory:" ) );
  txt_outdir->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_outdir, grow, 0, 1, 1 );

  m_outputDirectory = new QLineEdit ( this );
  m_outputDirectory->setObjectName ( QLatin1String ( "Targets/Directory" ) );
  m_outputDirectory->setText ( QString::fromUtf8 ( "/tmp" ) );
  /*: WhatsThis */
  m_outputDirectory->setWhatsThis ( trUtf8 ( "directory where to store output files" ) );
  gridLayout->addWidget ( m_outputDirectory, grow, 1, 1, 1 );

  QToolButton* setOutputBtn = new QToolButton ( this );
  setOutputBtn->setIcon ( folderIcon );
  gridLayout->addWidget ( setOutputBtn, grow++, 2, 1, 1 );
  // end: Output Directory

  gridLayout->setRowStretch ( grow, 1 );

  setLayout ( gridLayout );

  connect ( restoreButton, SIGNAL ( clicked() ),
            this, SLOT ( restoreFileName() ) );

  connect ( setFFmpegBtn, SIGNAL ( clicked() ),
            this, SLOT ( setFFmpegBinary() ) );

  connect ( m_outputName, SIGNAL ( textChanged ( const QString & ) ),
            this, SLOT ( valiadateInput ( const QString & ) ) );

  connect ( setOutputBtn, SIGNAL ( clicked() ),
            this, SLOT ( setOutpuDirectory() ) );
}

/**
* Setze den Pfad zum FFmpeg Programm
*/
void TargetsWidget::setFFmpegBinary()
{
  QString path ( "/usr/bin" );
  QStringList filt;
  filt << QString::fromUtf8 ( "FFmpeg %1" ).arg ( "(ffmpeg* *ffmpeg)" );
  // NOTE avconv from http://libav.org didnt support pulseaudio
  filt << QString::fromUtf8 ( "avconv %1 %2" ).arg ( trUtf8 ( "without pulse" ), "(avconv* *avconv)" );
  filt << trUtf8 ( "Other %1" ).arg ( "*" );

  /*: WindowTitle */
  path = QFileDialog::getOpenFileName ( this, trUtf8 ( "FFmpeg Binary" ), path, filt.join ( ";;" ) );

  QFileInfo db ( path );
  if ( db.exists() )
  {
    m_binaryFile->setText ( db.absoluteFilePath() );
    emit postUpdate ( true );
  }
}

void TargetsWidget::valiadateInput ( const QString &name )
{
  Q_UNUSED ( name );
  emit postUpdate ( true );
}

/**
* Setze den Pfad zum Ausgabe Verzeichnis
*/
void TargetsWidget::setOutpuDirectory()
{
  QString path ( "/tmp" );
  /*: WindowTitle */
  path = QFileDialog::getExistingDirectory ( this, trUtf8 ( "Output Directory" ), path );

  QFileInfo db ( path );
  if ( db.isWritable() )
  {
    m_outputDirectory->setText ( path );
    emit postUpdate ( true );
  }
}

/**
* Ausgabename zurück setzen!
*/
void TargetsWidget::restoreFileName()
{
  m_outputName->setText ( "qx11grab-XXXXXX" );
  emit postUpdate ( true );
}

/**
* Einstellungen Laden
*/
void TargetsWidget::load ( Settings * cfg )
{
  QList<QLineEdit*> items;
  items << m_outputDirectory << m_outputName << m_binaryFile;

  foreach ( QLineEdit* edit, items )
  {
    edit->setText ( cfg->value ( edit->objectName(), edit->text() ).toString() );
  }
}

/**
* Einstellungen Speichern
*/
void TargetsWidget::save ( Settings * cfg )
{
  QList<QLineEdit*> items;
  items << m_outputDirectory << m_outputName << m_binaryFile;

  foreach ( QLineEdit* edit, items )
  {
    if ( edit->text().isEmpty() )
      cfg->remove ( edit->objectName() );
    else
      cfg->setValue ( edit->objectName(), edit->text() );
  }
}

TargetsWidget::~TargetsWidget()
{}

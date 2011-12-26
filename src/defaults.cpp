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

#include "defaults.h"
#include "audiodevicewidget.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

/* QtCore */
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QTime>

/* QtGui */
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>

Defaults::Defaults ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "defaults" ) );
  setBackgroundRole ( QPalette::Window );

  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 2, 2, 2, 2 );
  gridLayout->setObjectName ( QLatin1String ( "gridLayout" ) );

  QIcon folderIcon = getThemeIcon ( "folder-open" );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  int gridRow = 0;
  // begin: FFmpeg Binary
  QLabel* txt_ffbinary = new QLabel ( this );
  txt_ffbinary->setText ( trUtf8 ( "FFmpeg Application:" ) );
  txt_ffbinary->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_ffbinary, gridRow, 0, 1, 1 );

  ff_path = new QLineEdit ( this );
  ff_path->setObjectName ( QLatin1String ( "ff_path" ) );
  ff_path->setText ( QLatin1String ( "/usr/bin/ffmpeg" ) );
  /*: WhatsThis */
  ff_path->setWhatsThis ( trUtf8 ( "absolute path to FFmpeg's recording application" ) );
  gridLayout->addWidget ( ff_path, gridRow, 1, 1, 1 );

  QToolButton* setFFmpegBtn = new QToolButton ( this );
  setFFmpegBtn->setIcon ( folderIcon );
  gridLayout->addWidget ( setFFmpegBtn, gridRow, 2, 1, 1 );
  // end: FFmpeg Binary

  // begin: Output Filename
  QLabel* txt_outfile = new QLabel ( this );
  txt_outfile->setText ( trUtf8 ( "Output Filename:" ) );
  txt_outfile->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_outfile, ++gridRow, 0, 1, 1 );

  outputName = new QLineEdit ( this );
  outputName->setObjectName ( QLatin1String ( "outputName" ) );
  outputName->setText ( QString::fromUtf8 ( "qx11grab-XXXXXX" ) );
  /*: WhatsThis */
  outputName->setWhatsThis ( trUtf8 ( "Specified output filenames can contain the following template XXXXXX (six upper case \"X\" characters), which will be replaced by the auto-generated portion of the filename." ) );
  gridLayout->addWidget ( outputName, gridRow, 1, 1, 1 );

  QToolButton* restoreButton = new QToolButton ( this );
  restoreButton->setIcon ( QIcon::fromTheme ( "edit-clear-locationbar-rtl" ) );
  gridLayout->addWidget ( restoreButton, gridRow, 2, 1, 1 );
  // end: Output Filename

  // begin: Output Directory
  QLabel* txt_outdir = new QLabel ( this );
  txt_outdir->setText ( trUtf8 ( "Output Directory:" ) );
  txt_outdir->setAlignment ( labelAlignment );
  gridLayout->addWidget ( txt_outdir, ++gridRow, 0, 1, 1 );

  outputDirectory = new QLineEdit ( this );
  outputDirectory->setObjectName ( QLatin1String ( "tempdir" ) );
  outputDirectory->setText ( QString::fromUtf8 ( "/tmp" ) );
  /*: WhatsThis */
  outputDirectory->setWhatsThis ( trUtf8 ( "directory where to store output files" ) );
  gridLayout->addWidget ( outputDirectory, gridRow, 1, 1, 1 );

  QToolButton* setOutputBtn = new QToolButton ( this );
  setOutputBtn->setIcon ( folderIcon );
  gridLayout->addWidget ( setOutputBtn, gridRow, 2, 1, 1 );
  // end: Output Directory

  // begin: Audio Device
  m_audioDeviceWidget = new AudioDeviceWidget ( this );
  gridLayout->addWidget ( m_audioDeviceWidget, ++gridRow, 0, 1, 3 );
  // end: Audio Device

  QSpacerItem* spacer  = new QSpacerItem ( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
  gridLayout->addItem ( spacer, ++gridRow, 0, 1, 2 );

  setLayout ( gridLayout );

  connect ( restoreButton, SIGNAL ( clicked() ), this, SLOT ( restoreFileName() ) );
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
void Defaults::restoreFileName()
{
  outputName->setText ( "qx11grab-XXXXXX" );
}

/**
* Einstellungen Laden
*/
void Defaults::load ( QSettings * cfg )
{
  QList<QLineEdit*> items;
  items << outputDirectory << outputName << ff_path;
  m_audioDeviceWidget->setVolume ( cfg->value ( QLatin1String ( "audio_intensifier" ), 256 ).toUInt() );

  m_audioDeviceWidget->setAudioDevice ( cfg->value ( QLatin1String ( "audio_device" )
                                        , QLatin1String ( "default" ) ).toString() );

  m_audioDeviceWidget->setAudioEngine ( cfg->value ( QLatin1String ( "audio_engine" )
                                        , QLatin1String ( "alsa" ) ).toString() );

  m_audioDeviceWidget->setSampleFormat ( cfg->value ( QLatin1String ( "audio_sample_fmt" )
                                         , QLatin1String ( "NONE" ) ).toString() );

  m_audioDeviceWidget->setAudioServiceType ( cfg->value ( QLatin1String ( "audio_service_type" )
          , QLatin1String ( "ma" ) ).toString() );

  // insert leinedits
  foreach ( QLineEdit* edit, items )
  {
    edit->setText ( cfg->value ( edit->objectName(), edit->text() ).toString() );
  }
}

/**
* Einstellungen Speichern
*/
void Defaults::save ( QSettings * cfg )
{
  QList<QLineEdit*> items;
  items << outputDirectory << outputName << ff_path;

  cfg->setValue ( QLatin1String ( "audio_engine" ), m_audioDeviceWidget->getAudioEngine() );

  if ( m_audioDeviceWidget->getVolume() != 256 )
    cfg->setValue ( QLatin1String ( "audio_intensifier" ), m_audioDeviceWidget->getVolume() );
  else
    cfg->remove ( QLatin1String ( "audio_intensifier" ) );

  if ( m_audioDeviceWidget->getAudioDevice().isEmpty() )
    cfg->remove ( QLatin1String ( "audio_device" ) );
  else
    cfg->setValue ( QLatin1String ( "audio_device" ), m_audioDeviceWidget->getAudioDevice() );

  if ( m_audioDeviceWidget->getSampleFormat().isEmpty() )
    cfg->remove ( QLatin1String ( "audio_sample_fmt" ) );
  else
    cfg->setValue ( QLatin1String ( "audio_sample_fmt" ), m_audioDeviceWidget->getSampleFormat() );

  if ( m_audioDeviceWidget->getAudioServiceType().isEmpty() )
    cfg->remove ( QLatin1String ( "audio_service_type" ) );
  else
    cfg->setValue ( QLatin1String ( "audio_service_type" ), m_audioDeviceWidget->getAudioServiceType() );

  foreach ( QLineEdit* edit, items )
  {
    if ( edit->text().isEmpty() )
      cfg->remove ( edit->objectName() );
    else
      cfg->setValue ( edit->objectName(), edit->text() );
  }
}

const QString Defaults::binary()
{
  return ff_path->text();
}

const QStringList Defaults::audioDeviceData()
{
  return  m_audioDeviceWidget->data();
}

const QString Defaults::output()
{
  QString p = outputDirectory->text();
  p.append ( "/" );

  QString outFile = outputName->text();
  QString timeStamp = QTime::currentTime().toString ( "hhmmss" );
  outFile.replace ( QRegExp ( "\\b(X{3,})\\b" ), timeStamp );
  p.append ( outFile );
  return p;
}

Defaults::~Defaults()
{}

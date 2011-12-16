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

#include "audiodevicewidget.h"
#include "audiodevice.h"
#include "alsaaudiodialog.h"
#include "pulseaudiodialog.h"

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

AudioDeviceWidget::AudioDeviceWidget ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "audiodevice" ) );
  setContentsMargins ( 0, 2, 0, 2 );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );
  vLayout->setContentsMargins ( 0, 2, 0, 2 );

  QGroupBox* audioGroup = new QGroupBox ( trUtf8 ( "Audio Settings" ), this );
  audioGroup->setFlat ( true );

  int grow = 0; // GridLayout RowCount
  QGridLayout* gridLayout = new QGridLayout ( audioGroup );
  m_swapAudio = new QComboBox ( audioGroup );
  m_swapAudio->insertItem ( NONE, trUtf8 ( "Audio System" ) );
  m_swapAudio->insertItem ( ALSA, trUtf8 ( "Advanced Linux Sound Architecture (Alsa)" ) );
  m_swapAudio->insertItem ( OSS, trUtf8 ( "Open Sound System (OSS)" ) );
  m_swapAudio->insertItem ( PULSE, trUtf8 ( "Soundserver (Pulse)" ) );
  m_swapAudio->setCurrentIndex ( 0 );
  gridLayout->addWidget ( m_swapAudio, grow++, 0, 1, 3, Qt::AlignRight );

  QLabel* txt0 = new QLabel ( audioGroup );
  txt0->setText ( trUtf8 ( "Audio Intensifier:" ) );
  txt0->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt0, grow, 0, 1, 1 );

  intensifier = new QSpinBox ( audioGroup );
  intensifier->setSingleStep ( 2 );
  intensifier->setRange ( 0, 512 );
  intensifier->setValue ( 256 );
  intensifier->setObjectName ( QLatin1String ( "audio_intensifier" ) );
  intensifier->setToolTip ( trUtf8 ( "Change Audio Volume (256=normal)" ) );
  intensifier->setWhatsThis ( trUtf8 ( "Change Audio Amplifier.\nDefault: 256=normal" ) );
  gridLayout->addWidget ( intensifier, grow++, 1, 1, 2 );

  QLabel* txt1 = new QLabel ( audioGroup );
  txt1->setText ( trUtf8 ( "Audio Capture Device:" ) );
  txt1->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt1, grow, 0, 1, 1 );

  device = new QLineEdit ( audioGroup );
  device->setObjectName ( QLatin1String ( "device" ) );
  device->setWhatsThis ( trUtf8 ( "Change Audio Capture Device\nOSS Default: /dev/dsp or Alsa Default: default" ) );
  gridLayout->addWidget ( device, grow, 1, 1, 1 );

  findAlsaPCMButton = new QToolButton ( audioGroup );
  findAlsaPCMButton->setObjectName ( QLatin1String ( "findalsapcmbutton" ) );
  findAlsaPCMButton->setIcon ( getThemeIcon ( "audio-headset" ) );
  findAlsaPCMButton->setWhatsThis ( trUtf8 ( "Select ALSA Audio Capture Device" ) );
  findAlsaPCMButton->setDisabled ( true ); // Standard ist Deaktiviert
  gridLayout->addWidget ( findAlsaPCMButton, grow++, 2, 1, 1 );

  QLabel* txt2 = new QLabel ( audioGroup );
  txt2->setText ( trUtf8 ( "Sample Format:" ) );
  txt2->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt2, grow, 0, 1, 1 );

  m_audioSampleFormat = new QComboBox ( audioGroup );
  m_audioSampleFormat->setObjectName ( QLatin1String( "sample_fmt" ) );
  m_audioSampleFormat->setToolTip ( trUtf8 ( "The sample format of the incoming audio buffers." ) );
  m_audioSampleFormat->setWhatsThis ( trUtf8 ( "To show available sample formats use ffmpeg -sample_fmts" ) );
  int sfIndex = 0;
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "unknown" ), QVariant ( "NONE" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "signed  8 bits" ), QVariant ( "u8" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "signed 16 bits" ), QVariant ( "s16" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "signed 32 bits" ), QVariant ( "s32" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "float" ), QVariant ( "flt" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "double" ), QVariant ( "dbl" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "unsigned  8 bits, planar" ), QVariant ( "u8p" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "signed 16 bits, planar" ), QVariant ( "s16p" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "signed 32 bits, planar" ), QVariant ( "s32p" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "float, planar" ), QVariant ( "fltp" ) );
  m_audioSampleFormat->insertItem ( sfIndex++, trUtf8 ( "double, planar" ), QVariant ( "dblp" ) );
  m_audioSampleFormat->setCurrentIndex ( 0 );
  gridLayout->addWidget ( m_audioSampleFormat, grow++, 1, 1, 2, Qt::AlignLeft );

  vLayout->addWidget ( audioGroup );
  vLayout->addStretch ( 1 );

  setLayout ( vLayout );

  connect ( m_swapAudio, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( audioEngineChanged ( int ) ) );

  connect ( findAlsaPCMButton, SIGNAL ( clicked () ),
            this, SLOT ( getpcmClicked() ) );
}

/**
* Öffnet den Dialog für das setzen der ALSA
* PCM Schnittstellen.
*/
void AudioDeviceWidget::openAlsaDialog()
{
  AlsaAudioDialog* dialog = new AlsaAudioDialog ( this );
  dialog->setCard ( device->text() );
  if ( dialog->exec() == QDialog::Accepted )
  {
    AudioDevice d = dialog->cardInfo();
    device->setText ( d.name );
    device->setToolTip ( d.hw );
    device->setStatusTip ( d.description );
  }
  delete dialog;
}

/**
* Öffnet den Dialog für das setzen der Pulse source Schnittstellen.
*/
void AudioDeviceWidget::openPulseDialog()
{
  PulseAudioDialog* dialog = new PulseAudioDialog ( this );
  dialog->setCard ( device->text() );
  if ( dialog->exec() == QDialog::Accepted )
  {
    AudioDevice d = dialog->cardInfo();
    device->setText ( d.name );
    device->setToolTip ( d.hw );
    device->setStatusTip ( d.description );
  }
  delete dialog;
}

/**
* Interne SLOT der ermittelt welches Audio System
* gerade ausgewählt wurde.
*/
void AudioDeviceWidget::getpcmClicked()
{
  switch ( m_swapAudio->currentIndex() )
  {
    case NONE:
      break;

    case ALSA:
      openAlsaDialog();
      break;

    case OSS:
      break;

    case PULSE:
      openPulseDialog();
      break;

    default:
      return;
  };
}

/**
* Setzt den Wert für die Audio-Verstärkung
*/
void AudioDeviceWidget::setVolume ( int i )
{
  intensifier->setValue ( i );
}

/**
* Gibt den aktuellen Status der Audio-Verstärkung zurück.
*/
int AudioDeviceWidget::getVolume ()
{
  return intensifier->value();
}

/**
* Setzt das Audio System ueber die Auswahl Box
*/
void AudioDeviceWidget::audioEngineChanged ( int index )
{
  switch ( index )
  {
    case NONE:
      findAlsaPCMButton->setDisabled ( true );
      break;

    case ALSA:
      findAlsaPCMButton->setDisabled ( false );
      break;

    case OSS:
      findAlsaPCMButton->setDisabled ( true );
      break;

    case PULSE:
      findAlsaPCMButton->setDisabled ( false );
      break;

    default:
      findAlsaPCMButton->setDisabled ( true );
      break;
  };
}

/**
* Setzt das Audio System, "alsa","oss" oder "pulse" sind erlaubt.
*/
void AudioDeviceWidget::setAudioEngine ( const QString &t )
{
  if ( t.contains ( QString::fromUtf8 ( "alsa" ) ) )
  {
    m_swapAudio->setCurrentIndex ( ALSA );
    findAlsaPCMButton->setDisabled ( false );
  }
  else if ( t.contains ( QString::fromUtf8 ( "oss" ) ) )
  {
    m_swapAudio->setCurrentIndex ( OSS );
    findAlsaPCMButton->setDisabled ( true );
  }
  else if ( t.contains ( QString::fromUtf8 ( "pulse" ) ) )
  {
    m_swapAudio->setCurrentIndex ( PULSE );
    findAlsaPCMButton->setDisabled ( false );
  }
}

/**
* Ermittelt den Aktuellen zustand der RadioBoxen und gibt
* das entsprechend ausgewählte Audio System zurück.
*/
const QString AudioDeviceWidget::getAudioEngine ()
{
  QString devType;
  switch ( m_swapAudio->currentIndex() )
  {
    case NONE:
      devType = trUtf8 ( "Unknown" );
      break;

    case ALSA:
      devType = QLatin1String ( "alsa" );
      break;

    case OSS:
      devType = QLatin1String ( "oss" );
      break;

    case PULSE:
      devType = QLatin1String ( "pulse" );
      break;

    default:
      devType = trUtf8 ( "Unknown" );
      break;
  };
  return devType;
}

void AudioDeviceWidget::setAudioDevice ( AudioDeviceWidget::AUDIODEV dev )
{
  m_swapAudio->setCurrentIndex ( dev );
}

/**
* Setzt unbeachtet dessen welches Audio System gerade
* ausgewählt ist, die Aufnahme Schnittstelle.
*/
void AudioDeviceWidget::setAudioDevice ( const QString &d )
{
  device->setText ( d );
}

/**
* Gibt unbeachtet dessen welches Audio System gerade
* ausgewählt ist, die Aufnahme Schnittstelle zurück.
*/
const QString AudioDeviceWidget::getAudioDevice()
{
  return device->text();
}

/**
* Setz das Aktuelle Sample Format
*/
void AudioDeviceWidget::setSampleFormat ( const QString &sfmt )
{
  for ( int i = 0; i < m_audioSampleFormat->count(); ++i )
  {
    if ( m_audioSampleFormat->itemData ( i, Qt::UserRole ).toString().compare ( sfmt ) == 0 )
    {
      m_audioSampleFormat->setCurrentIndex ( i );
      break;
    }
  }
}

/**
* Hole das Sample Format
*/
const QString AudioDeviceWidget::getSampleFormat()
{
  QString sample_fmt;
  sample_fmt = m_audioSampleFormat->itemData ( m_audioSampleFormat->currentIndex(), Qt::UserRole ).toString();
  return sample_fmt;
}

/**
* Erzeugt die Kommandozeilen Zeichenkette für die
* FFmpeg Argumenten-Liste der Audio Aufnahme.
*/
const QStringList AudioDeviceWidget::data()
{
  QStringList cmd;
  if ( device->text().isEmpty() )
    return cmd;

  switch ( m_swapAudio->currentIndex() )
  {
    case ALSA:
      cmd << "-f" << "alsa" << "-i" << device->text();
      break;

    case OSS:
      cmd << "-f" << "oss" << "-i" << device->text();
      break;

    case PULSE:
      cmd << "-f" << "pulse" << "-i" << ( device->text().isEmpty() ? "default" : device->text() );
      break;

    default:
      break;
  };

  if ( ! getSampleFormat().contains ( "NONE" ) )
    cmd<< "-sample_fmt" << getSampleFormat();

  if ( intensifier->value() != 256 )
    cmd << "-vol" << QString::number ( intensifier->value() );

  return cmd;
}

AudioDeviceWidget::~AudioDeviceWidget()
{}

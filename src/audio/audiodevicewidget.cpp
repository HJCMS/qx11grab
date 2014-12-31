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

#include "audiodevicewidget.h"
#include "audiodevice.h"
#include "alsaaudiodialog.h"
#include "opensounddialog.h"
#ifdef HAVE_PULSE
# include "pulseaudiodialog.h"
#endif

#ifndef QX11GRAB_VERSION
#include "version.h"
#endif

/* QtCore */
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QString>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QIcon>
#include <QtGui/QVBoxLayout>

AudioDeviceWidget::AudioDeviceWidget ( QWidget * parent )
    : QGroupBox ( parent )
{
  setObjectName ( QLatin1String ( "audiodevice" ) );
  setTitle ( trUtf8 ( "Audio Device" ) );
  setFlat ( true );
  setCheckable ( false );

  int grow = 0; // GridLayout RowCount
  QGridLayout* gridLayout = new QGridLayout ( this );
  gridLayout->setContentsMargins ( 0, 2, 0, 2 );

  m_swapAudio = new QComboBox ( this );
  /*: WhatsThis */
  m_swapAudio->setWhatsThis ( trUtf8 ( "choose the audio interface to use" ) );
  m_swapAudio->insertItem ( NONE, trUtf8 ( "Audio System" ) );
  m_swapAudio->insertItem ( ALSA, trUtf8 ( "Advanced Linux Sound Architecture (Alsa)" ) );
  m_swapAudio->insertItem ( OSS, trUtf8 ( "Open Sound System (OSS)" ) );
#ifdef HAVE_PULSE
  m_swapAudio->insertItem ( PULSE, trUtf8 ( "Soundserver (Pulse)" ) );
#endif
  m_swapAudio->setCurrentIndex ( 0 );
  gridLayout->addWidget ( m_swapAudio, grow++, 0, 1, 3, Qt::AlignRight );

  // Volume {
  QLabel* txt0 = new QLabel ( this );
  txt0->setText ( trUtf8 ( "Audio Intensifier:" ) );
  txt0->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt0, grow, 0, 1, 1 );

  m_slider = new QSlider ( Qt::Horizontal, this );
  m_slider->setObjectName ( QLatin1String ( "AudioSlider" ) );
  /*: ToolTip */
  m_slider->setToolTip ( trUtf8 ( "Change Audio Volume +/- 15 dB" ) );
  /*: WhatsThis */
  m_slider->setWhatsThis ( trUtf8 ( "Set an initial volume, in dB, to be assumed for each channel when filtering starts." ) );
  m_slider->setSingleStep ( 1 );
  m_slider->setRange ( -15, +15 );
  m_slider->setTickPosition ( QSlider::TicksAbove );
  m_slider->setValue ( 0 );
  gridLayout->addWidget ( m_slider, grow, 1, 1, 1 );
  m_volume = new QLabel ( this );
  m_volume->setWordWrap ( false );
  gridLayout->addWidget ( m_volume, grow++, 2, 1, 1 );
  // } Volume

  QLabel* txt1 = new QLabel ( this );
  txt1->setText ( trUtf8 ( "Audio Capture Device:" ) );
  txt1->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt1, grow, 0, 1, 1 );

  device = new QLineEdit ( this );
  device->setObjectName ( QLatin1String ( "device" ) );
  /*: WhatsThis */
  device->setWhatsThis ( trUtf8 ( "Audio Capture Device\nExample for OSS: /dev/dsp" ) );
  gridLayout->addWidget ( device, grow, 1, 1, 1 );

  m_audiodevButton = new QToolButton ( this );
  m_audiodevButton->setIcon ( QIcon::fromTheme ( "audio-headset" ) );
  m_audiodevButton->setWhatsThis ( trUtf8 ( "Open dialog to select audio capture device" ) );
  m_audiodevButton->setDisabled ( true ); // Standard ist Deaktiviert
  gridLayout->addWidget ( m_audiodevButton, grow++, 2, 1, 1 );

  // sample_fmt {
  QLabel* txt2 = new QLabel ( this );
  txt2->setText ( trUtf8 ( "Sample Format:" ) );
  txt2->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt2, grow, 0, 1, 1 );

  m_audioSampleFormat = new QComboBox ( this );
  m_audioSampleFormat->setObjectName ( QLatin1String ( "sample_fmt" ) );
  /*: ToolTip */
  m_audioSampleFormat->setToolTip ( trUtf8 ( "Audio sample format" ) );
  /*: WhatsThis */
  m_audioSampleFormat->setWhatsThis ( trUtf8 ( "The sample format of the incoming audio buffers." ) );

  int it_index = 0;
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "Automatically" ), QVariant ( "NONE" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "signed  8 bits" ), QVariant ( "u8" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "signed 16 bits" ), QVariant ( "s16" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "signed 32 bits" ), QVariant ( "s32" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "float" ), QVariant ( "flt" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "double" ), QVariant ( "dbl" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "unsigned  8 bits, planar" ), QVariant ( "u8p" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "signed 16 bits, planar" ), QVariant ( "s16p" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "signed 32 bits, planar" ), QVariant ( "s32p" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "float, planar" ), QVariant ( "fltp" ) );
  m_audioSampleFormat->insertItem ( it_index++, trUtf8 ( "double, planar" ), QVariant ( "dblp" ) );
  m_audioSampleFormat->setCurrentIndex ( 0 );
  gridLayout->addWidget ( m_audioSampleFormat, grow++, 1, 1, 2, Qt::AlignLeft );
  // } sample_fmt

  // audio_service_type {
  QLabel* txt3 = new QLabel ( this );
  /*: Note: short */
  txt3->setText ( trUtf8 ( "Audio Service Type:" ) );
  txt3->setAlignment ( ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter ) );
  gridLayout->addWidget ( txt3, grow, 0, 1, 1 );

  m_audioServiceType = new QComboBox ( this );
  m_audioServiceType->setObjectName ( QLatin1String ( "audio_service_type" ) );
  /*: ToolTip */
  m_audioServiceType->setToolTip ( trUtf8 ( "Audio service type" ) );
  /*: WhatsThis */
  m_audioServiceType->setWhatsThis ( trUtf8 ( "The type of service that the audio stream contains." ) );

  it_index = 0;
  /*: Alternative: Default Audio */
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Main Audio Service" ), QVariant ( "ma" ) );
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Effects" ), QVariant ( "ef" ) );
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Visually Impaired" ), QVariant ( "vi" ) );
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Hearing Impaired" ), QVariant ( "hi" ) );
  /*: Alternative: dialogue level */
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Dialogue" ), QVariant ( "di" ) );
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Commentary" ), QVariant ( "co" ) );
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Emergency" ), QVariant ( "em" ) );
  /*: Alternative: telephony */
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Voice Over" ), QVariant ( "vo" ) );
  m_audioServiceType->insertItem ( it_index++, trUtf8 ( "Karaoke" ), QVariant ( "ka" ) );

  gridLayout->addWidget ( m_audioServiceType, grow++, 1, 1, 2, Qt::AlignLeft );
  // } audio_service_type

  gridLayout->setRowStretch ( grow++, 1 );

  setLayout ( gridLayout );

  connect ( m_swapAudio, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( audioEngineChanged ( int ) ) );

  connect ( m_audiodevButton, SIGNAL ( clicked () ),
            this, SLOT ( getpcmClicked() ) );

  // Updates
  connect ( m_slider, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );
  connect ( m_slider, SIGNAL ( valueChanged ( int ) ),
            this, SLOT ( audioVolumeChanged ( int ) ) );

  connect ( m_audioSampleFormat, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );

  connect ( m_audioServiceType, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( integerUpdate ( int ) ) );
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
    emit postUpdate ( true );
  }
  delete dialog;
}

/**
* Öffnet den Dialog für das setzen der OSS Schnittstellen.
*/
void AudioDeviceWidget::openOSSDialog()
{
  OpenSoundDialog* dialog = new OpenSoundDialog ( this );
  dialog->setCard ( device->text() );
  if ( dialog->exec() == QDialog::Accepted )
  {
    AudioDevice d = dialog->cardInfo();
    device->setText ( d.name );
    device->setToolTip ( d.hw );
    device->setStatusTip ( d.description );
    emit postUpdate ( true );
  }
  delete dialog;
}

/**
* Öffnet den Dialog für das setzen der Pulse source Schnittstellen.
*/
void AudioDeviceWidget::openPulseDialog()
{
#ifdef HAVE_PULSE
  PulseAudioDialog* dialog = new PulseAudioDialog ( this );
  dialog->setCard ( device->text() );
  if ( dialog->exec() == QDialog::Accepted )
  {
    AudioDevice d = dialog->cardInfo();
    device->setText ( d.name );
    device->setToolTip ( d.hw );
    device->setStatusTip ( d.description );
    emit postUpdate ( true );
  }
  delete dialog;
#endif
}

/** Ein Eingabe Status wurde geändert */
void AudioDeviceWidget::statusUpdate ( bool )
{
  emit postUpdate ( true );
}

/** Ein Eingabe Status wurde geändert */
void AudioDeviceWidget::integerUpdate ( int )
{
  emit postUpdate ( true );
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
      openOSSDialog();
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
  m_slider->setValue ( i );
  audioVolumeChanged ( i );
}

/**
* Gibt den aktuellen Status der Audio-Verstärkung zurück.
*/
int AudioDeviceWidget::getVolume ()
{
  return m_slider->value();
}

void AudioDeviceWidget::audioVolumeChanged ( int index )
{
  QString suffix = ( index>0 ) ? "+" : "";
  QString txt = QString ( "%1%2 dB" ).arg ( suffix,QString::number ( index ) );
  m_volume->setText ( txt );
}

/**
* Setzt das Audio System ueber die Auswahl Box
*/
void AudioDeviceWidget::audioEngineChanged ( int index )
{
  switch ( index )
  {
    case NONE:
      m_audiodevButton->setDisabled ( true );
      break;

    case ALSA:
      m_audiodevButton->setDisabled ( false );
      break;

    case OSS:
      m_audiodevButton->setDisabled ( false );
      break;

    case PULSE:
      m_audiodevButton->setDisabled ( false );
      break;

    default:
      m_audiodevButton->setDisabled ( true );
      break;
  };
  emit postUpdate ( true );
}

/**
* Setzt das Audio System, "alsa","oss" oder "pulse" sind erlaubt.
*/
void AudioDeviceWidget::setAudioEngine ( const QString &t )
{
  if ( t.contains ( QString::fromUtf8 ( "alsa" ) ) )
  {
    m_swapAudio->setCurrentIndex ( ALSA );
    m_audiodevButton->setDisabled ( false );
  }
  else if ( t.contains ( QString::fromUtf8 ( "oss" ) ) )
  {
    m_swapAudio->setCurrentIndex ( OSS );
    m_audiodevButton->setDisabled ( true );
  }
  else if ( t.contains ( QString::fromUtf8 ( "pulse" ) ) )
  {
    m_swapAudio->setCurrentIndex ( PULSE );
    m_audiodevButton->setDisabled ( false );
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
* \category AdvancedAudioOption
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
* \category AdvancedAudioOption
*/
const QString AudioDeviceWidget::getSampleFormat()
{
  QString sample_fmt;
  sample_fmt = m_audioSampleFormat->itemData ( m_audioSampleFormat->currentIndex(), Qt::UserRole ).toString();
  return sample_fmt;
}

/**
* Setz den Aktuellen Audio Schnittstellen Type
* \category AdvancedAudioOption
*/
void AudioDeviceWidget::setAudioServiceType ( const QString &ast )
{
  for ( int i = 0; i < m_audioServiceType->count(); ++i )
  {
    if ( m_audioServiceType->itemData ( i, Qt::UserRole ).toString().compare ( ast ) == 0 )
    {
      m_audioServiceType->setCurrentIndex ( i );
      break;
    }
  }
}

/**
* Audio Schnittstellen Type
* \category AdvancedAudioOption
*/
const QString AudioDeviceWidget::getAudioServiceType()
{
  QString audio_service_type;
  audio_service_type = m_audioServiceType->itemData ( m_audioServiceType->currentIndex(), Qt::UserRole ).toString();
  return audio_service_type;
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
      cmd << "-f" << "alsa" << "-i" << ( device->text().isEmpty() ? "default" : device->text() );
      break;

    case OSS:
      cmd << "-f" << "oss" << "-i" << ( device->text().isEmpty() ? "/dev/dsp" : device->text() );
      break;

    case PULSE:
      cmd << "-f" << "pulse" << "-i" << ( device->text().isEmpty() ? "default" : device->text() );
      break;

    default:
      break;
  };

  if ( ! getSampleFormat().contains ( "NONE" ) )
    cmd<< "-sample_fmt" << getSampleFormat();

  if ( ! getAudioServiceType().isEmpty () )
    cmd<< "-audio_service_type" << getAudioServiceType();

  if ( m_slider->value() != 0 )
  {
    QString suffix = ( m_slider->value() >0 ) ? "+" : "";
    cmd << QString::fromUtf8 ( "-filter:a volume=%1%2" ).arg ( suffix, QString::number ( m_slider->value() ) );
  }

  return cmd;
}

AudioDeviceWidget::~AudioDeviceWidget()
{}

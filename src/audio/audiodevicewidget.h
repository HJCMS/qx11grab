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

#ifndef AUDIODEVICEWIDGET_H
#define AUDIODEVICEWIDGET_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

class AudioDeviceWidget : public QGroupBox
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_ENUMS ( AUDIODEV )

  private:
    QComboBox* m_swapAudio;
    QSlider* m_slider;
    QLineEdit* device;
    QToolButton* m_audiodevButton;
    QComboBox* m_audioSampleFormat;
    QComboBox* m_audioServiceType;

    void openAlsaDialog();
    void openPulseDialog();

  private Q_SLOTS:
    void statusUpdate ( bool );
    void integerUpdate ( int );
    void getpcmClicked();
    void audioEngineChanged ( int index );

  Q_SIGNALS:
    /**
    * this signal is emitted when items updated
    */
    void postUpdate( bool );

  public:
    explicit AudioDeviceWidget ( QWidget * parent = 0 );

    /**
    * indexes for audio engine ComboBox
    */
    enum AUDIODEV
    {
      NONE = 0, /**< Dummy for Experts only */
      ALSA = 1, /**< Advanced Linux Sound Architecture (alsa) */
      OSS = 2,  /**< Open Sound System (oss) */
      PULSE = 3 /**< Soundserver (pulse) */
    };

    /**
     * set audio itensifier
     * \param i Itensifier level
     */
    void setVolume ( int i );

    /**
     * get current audio itensifier
     */
    int getVolume ();

    /**
     * find ComboBox item with name for select index
     * \param t Audio Engine Type \see AUDIODEV
     */
    void setAudioEngine ( const QString &t );

    /**
     * read selected audio engine from ComboBox
     */
    const QString getAudioEngine ();

    /**
     * set current audio engine index in ComboBox to LineEdit
     * \param dev Audio Engine Type
     */
    void setAudioDevice ( AudioDeviceWidget::AUDIODEV dev );

    /**
     * force audio device and ignore selected ComboBox item
     * \param d Device Definition.
     * \note The Default for OSS is \e "/dev/dsp", Alsa and Pulse \e "default".
     */
    void setAudioDevice ( const QString &d );

    /**
     * read selected audio device from LineEdit
     */
    const QString getAudioDevice ();

    /**
     * Set the audio sample format.
     * \see ffmpeg -sample_fmts
     * \param sfmt sample format
     */
    void setSampleFormat ( const QString &sfmt );

    /**
     * Get the audio sample format.
     * \see ffmpeg -sample_fmt
     */
    const QString getSampleFormat();

    /**
     * Set the audio service type
     * \see ffmpeg -audio_service_type
     * \param ast type default:ma
     */
    void setAudioServiceType ( const QString &ast );

    /**
     * Get the audio service type
     * \see ffmpeg -audio_service_type
     */
    const QString getAudioServiceType();

    /**
     * Configured commandline String
     */
    const QStringList data();

    ~AudioDeviceWidget();
};

#endif

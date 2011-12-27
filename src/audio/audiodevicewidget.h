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
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

class AudioDeviceWidget : public QWidget
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
    void integerUpdate ( int );
    void getpcmClicked();
    void audioEngineChanged ( int index );

  Q_SIGNALS:
    void postUpdate();

  public:
    enum AUDIODEV { NONE = 0, ALSA = 1, OSS = 2, PULSE = 3 };

    AudioDeviceWidget ( QWidget * parent = 0 );

    void setVolume ( int );
    int getVolume ();

    void setAudioEngine ( const QString & );
    const QString getAudioEngine ();

    void setAudioDevice ( AudioDeviceWidget::AUDIODEV dev );
    void setAudioDevice ( const QString & );
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

    virtual ~AudioDeviceWidget();
};

#endif

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

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

class AudioDevice : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann (Undefined)" )
    Q_CLASSINFO ( "URL", "http://qx11grab.hjcms.de" )

  private:
    QRadioButton* m_swap_alsa;
    QRadioButton* m_swap_oss;
    QSpinBox* intensifier;
    QLineEdit* device;
    QToolButton* findAlsaPCMButton;
    void setAlsaRecordingPCM();

  private Q_SLOTS:
    void getpcmClicked();

  public:
    AudioDevice ( QWidget * parent = 0 );
    void setVolume ( int );
    int getVolume ();
    void setAudioEngine ( const QString & );
    const QString getAudioEngine ();
    void setAudioDevice ( const QString & );
    const QString getAudioDevice ();
    const QStringList data();
    ~AudioDevice();
};

#endif

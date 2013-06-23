/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2013
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

#include "ffoptionsmenu.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QResource>
#include <QtCore/QStringList>

/* QtGui */
#include <QtGui/QAction>

/* QtDBus */
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

namespace QX11Grab
{
  static const QIcon ffmpeg_icon()
  {
    QIcon icon ( QString::fromUtf8 ( ":/images/ffmpeg.png" ) );
    return QIcon::fromTheme ( QLatin1String ( "ffmpeg" ), icon );
  }

  FFOptionsMenu::FFOptionsMenu ( QWidget * parent )
      : QMenu ( parent )
      , p_iconVideo ( QIcon::fromTheme ( QLatin1String ( "video-x-generic" ), ffmpeg_icon() ) )
      , p_iconAudio ( QIcon::fromTheme ( QLatin1String ( "audio-x-generic" ), ffmpeg_icon() ) )
  {
    setObjectName ( QLatin1String ( "FFOptionsMenu" ) );
    setIcon ( ffmpeg_icon() );
    setTitle ( trUtf8 ( "Options" ) );
    setEnabled ( false );

    m_videoMenu = addMenu ( p_iconVideo, trUtf8 ( "Video" ) );
    m_audioMenu = addMenu ( p_iconAudio, trUtf8 ( "Audio" ) );

    m_signalMapper = new QSignalMapper ( this );
    connect ( m_signalMapper, SIGNAL ( mapped ( const QString & ) ),
              this, SLOT ( optionTriggered ( const QString & ) ) );

    insertGlobalOptions();
  }

  /**
  * Video Menü Eintrag einfügen
  */
  void FFOptionsMenu::insertVideoItem ( const QString &value, const QString &tooltip )
  {
    QString option = QString::fromUtf8 ( "-%1" ).arg ( value );
    QString tip = ( tooltip.isEmpty() ? option : tooltip );
    QAction* ac = m_videoMenu->addAction ( p_iconVideo, option );
    ac->setToolTip ( tip );
    ac->setStatusTip ( tip );
    connect ( ac, SIGNAL ( triggered () ), m_signalMapper, SLOT ( map () ) );
    m_signalMapper->setMapping ( ac, option );
  }

  /**
  * Audio Menü Eintrag einfügen
  */
  void FFOptionsMenu::insertAudioItem ( const QString &value, const QString &tooltip )
  {
    QString option = QString::fromUtf8 ( "-%1" ).arg ( value );
    QString tip = ( tooltip.isEmpty() ? option : tooltip );
    QAction* ac = m_audioMenu->addAction ( p_iconAudio, option );
    ac->setToolTip ( tip );
    ac->setStatusTip ( tip );
    connect ( ac, SIGNAL ( triggered () ), m_signalMapper, SLOT ( map () ) );
    m_signalMapper->setMapping ( ac, option );
  }

  /**
  * Siehe
  */
  void FFOptionsMenu::insertGlobalOptions()
  {
    QStringList list;
    m_videoMenu->clear();
    insertVideoItem ( "vpre", trUtf8 ( "Set video preset." ) );
    insertVideoItem ( "pix_fmt", trUtf8 ( "Set pixel format." ) );
    insertVideoItem ( "sws_flags", trUtf8 ( "SwScaler flags." ) );
    insertVideoItem ( "strict", trUtf8 ( "How strictly to follow the standards." ) );
    insertVideoItem ( "bsf:v", trUtf8 ( "Set bitstream filters for matching stream." ) );
    insertVideoItem ( "bufsize", trUtf8 ( "Set video buffer verifier buffer size (in bits)." ) );

    m_audioMenu->clear();
    insertAudioItem ( "qscale:a", trUtf8 ( "Sets the quantizer to use" ) );
    insertAudioItem ( "bsf:a", trUtf8 ( "Set bitstream filters for matching stream." ) );
    insertAudioItem ( "ar", trUtf8 ( "Set the audio sampling frequency." ) );
    insertAudioItem ( "ac", trUtf8 ( "Set the number of audio channels." ) );
    insertAudioItem ( "apre", trUtf8 ( "Set audio preset." ) );
    insertAudioItem ( "sample_fmt", trUtf8 ( "Audio sample format." ) );

    setEnabled ( true );
  }

  /**
  * Ein Eintrag wurde ausgewählt.
  */
  void FFOptionsMenu::optionTriggered ( const QString &opt )
  {
    // TODO Standard Option erfragen
    emit optionChanged ( opt, QVariant() );
  }

  FFOptionsMenu::~FFOptionsMenu()
  {}
}  /* eof namespace QX11Grab */

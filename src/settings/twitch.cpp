/*
* This file is part of the qx11grab project
*
* Copyright (C) Juergen Heinemann (Undefined) http://qx11grab.hjcms.de, (C) 2007-2015
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

#include "twitch.h"
#include "version.h"
#include "audiodevicewidget.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QCompleter>

/* QtGui */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

/* QJson */
#include <qjson/parser.h>

Twitch::Twitch ( QWidget * parent )
    : AbstractConfigWidget ( parent )
{
  setObjectName ( QLatin1String ( "Twitch" ) );
  /*: GroupBoxTitle */
  setTitle ( trUtf8 ( "twitch.tv" ) );

  QGridLayout* layout = new QGridLayout ( this );
  layout->setContentsMargins ( 0, 5, 0, 0 );
  layout->setObjectName ( QLatin1String ( "ConfigTwitch/Layout" ) );

  Qt::Alignment labelAlignment = ( Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter );

  int row = 0;
  // begin:Config {
  QLabel* txt_key = new QLabel ( this );
  txt_key->setText ( trUtf8 ( "Streaming Key:" ) );
  txt_key->setAlignment ( labelAlignment );
  layout->addWidget ( txt_key, row, 0, 1, 1 );

  m_keyEdit = new QLineEdit ( this );
  m_keyEdit->setObjectName ( "Streamkey" );
  layout->addWidget ( m_keyEdit, row++, 1, 1, 1 );

  QLabel* txt_server = new QLabel ( this );
  txt_server->setText ( trUtf8 ( "Server:" ) );
  txt_server->setAlignment ( labelAlignment );
  layout->addWidget ( txt_server, row, 0, 1, 1 );

  m_serverEdit = new QLineEdit ( this );
  m_serverEdit->setObjectName ( "Server" );
  m_serverEdit->setToolTip ( trUtf8 ( "" ) );
  layout->addWidget ( m_serverEdit, row++, 1, 1, 1 );

  m_serverList = new QComboBox ( this );
  layout->addWidget ( m_serverList, row++, 0, 1, 2 );

  m_joinAudio = new QCheckBox ( trUtf8 ( "Always join micro output with system audio" ), this );
  m_joinAudio->setObjectName ( "Join" );
  m_joinAudio->setToolTip ( trUtf8 ( "Only available with pulseaudio" ) );
  layout->addWidget ( m_joinAudio, row++, 0, 1, 2 );

  m_audioDeviceWidget = new AudioDeviceWidget ( this );
  m_audioDeviceWidget->setAudioDevice ( AudioDeviceWidget::PULSE );
  m_audioDeviceWidget->enableSimpleEdit ( true );
  layout->addWidget ( m_audioDeviceWidget, row++, 0, 1, 2 );
  // } end:Config

  QLabel* txt_info = new QLabel ( this );
  txt_info->setWordWrap ( true );
  txt_info->setText ( trUtf8 ( "Don't forget to add <b>amix</b> Filter with <i>-filter_complex</i> into Audiotable.<br/>Example: <i>\"[1:a][2:a]amix=inputs=2\"</i> to join Audio1/Audio2 Devices into single Stream!" ) );
  layout->addWidget ( txt_info, row++, 0, 1, 2 );

  layout->setRowStretch ( row, 1 );
  setLayout ( layout );

  init_rtmp_servers();

  connect ( m_serverList, SIGNAL ( currentIndexChanged ( int ) ),
            this, SLOT ( switchServer ( int ) ) );
}

void Twitch::init_rtmp_servers()
{
  QNetworkAccessManager* manager = new QNetworkAccessManager ( this );
  connect ( manager, SIGNAL ( finished ( QNetworkReply * ) ),
            this, SLOT ( readyRead ( QNetworkReply * ) ) );
  QNetworkRequest req;
  req.setUrl ( TWITCH_JSON_SERVERS );
  req.setRawHeader ( "Accept", "application/json" );
  manager->get ( req );
}

void Twitch::switchServer ( int _i )
{
  m_serverEdit->setText ( m_serverList->itemData ( _i ).toString() );
}

void Twitch::readyRead ( QNetworkReply * _r )
{
  QJson::Parser parser;
  bool ok;

  if ( ( _r->error() != QNetworkReply::NoError ) || ( ! _r->isFinished() ) )
    return;

  // wget -O /tmp/ingests.js https://api.twitch.tv/kraken/ingests
  // cat /tmp/ingests.js | json_pp
  QByteArray _json = QByteArray::fromPercentEncoding ( _r->readAll() );
  QVariantMap result = parser.parse ( _json, &ok ).toMap();
  if ( !ok )
  {
    qDebug() << Q_FUNC_INFO << parser.errorString();
    return;
  }

  QStringList _cpl;
  m_serverList->clear();
  foreach ( QVariant _item, result["ingests"].toList() )
  {
    QVariantMap _arr = _item.toMap();
    if ( _arr["availability"].toUInt() == 1 )
    {
      QString _sub = _arr["url_template"].toString();
      _sub.remove ( QRegExp ( "\\{stream_key\\}" ) );
      _cpl << _sub;
      m_serverList->addItem ( _arr["name"].toString(), _sub );
    }
  }

  QCompleter *_comp = new QCompleter ( _cpl, m_serverEdit );
  _comp->setCaseSensitivity ( Qt::CaseInsensitive );
  _comp->setCompletionMode ( QCompleter::PopupCompletion );
  m_serverEdit->setCompleter ( _comp );
}

void Twitch::load ( Settings * cfg )
{
  m_keyEdit->setText ( cfg->value ( "Twitch/Streamkey", "" ).toString() );
  m_serverEdit->setText ( cfg->value ( "Twitch/Server", "rtmp://live-ams.twitch.tv/app/" ).toString() );
  m_joinAudio->setChecked ( cfg->value ( "Twitch/Join", false ).toBool () );
  m_audioDeviceWidget->setAudioEngine ( cfg->value ( "Twitch/AudioEngine", "pulse" ).toString() );
  m_audioDeviceWidget->setAudioDevice ( cfg->value ( "Twitch/AudioDevice", "" ).toString() );
}

void Twitch::save ( Settings * cfg )
{
  cfg->setValue ( "Twitch/Enabled", ( ( !m_keyEdit->text().isEmpty() ) && m_joinAudio->isChecked() ) );
  cfg->setValue ( "Twitch/Streamkey", m_keyEdit->text() );
  cfg->setValue ( "Twitch/Server", m_serverEdit->text() );
  cfg->setValue ( "Twitch/Join", m_joinAudio->isChecked() );
  cfg->setValue ( "Twitch/AudioEngine", m_audioDeviceWidget->getAudioEngine() );
  cfg->setValue ( "Twitch/AudioDevice", m_audioDeviceWidget->getAudioDevice() );
}

Twitch::~Twitch()
{}

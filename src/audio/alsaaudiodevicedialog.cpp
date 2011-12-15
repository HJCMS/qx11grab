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

#include "alsaaudiodevice.h"
#include "alsaaudiodevicedialog.h"

/* QtCore */
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/* QtGui */
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QListWidgetItem>
#include <QtGui/QVBoxLayout>

extern "C"
{
#include "asoundlib.h"
}

AlsaAudioDeviceDialog::AlsaAudioDeviceDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "picrecordinterface" ) );
  setWindowTitle ( trUtf8 ( "ALSA PCM Selection" ) );
  setMinimumWidth ( 400 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  vLayout->addWidget ( new QLabel ( trUtf8 ( "Available Cards:" ) ) );

  deviceList = new QListWidget ( this );
  vLayout->addWidget ( deviceList );

  QDialogButtonBox* box = new QDialogButtonBox ( Qt::Horizontal , this );
  box->setStandardButtons ( ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel ) );
  box->setCenterButtons ( true );
  vLayout->addWidget ( box );

  setLayout ( vLayout );

  initRecorderDevices();

  connect ( box, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( box, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

void AlsaAudioDeviceDialog::initRecorderDevices()
{
  void **hints, **n;
  char *name;
  QString hwIndex;
  QRegExp pattern ( "^.+=" );
  QRegExp cleaner ( "[\\n\\r]+.+$" );
  QStringList buffer;

  /** @ref http://www.alsa-project.org/alsa-doc/alsa-lib/group___control.html
  * zero if success, otherwise a negative error code
  */
  if ( snd_device_name_hint ( -1, "pcm", &hints ) < 0 )
  {
    qWarning ( "no alsa devices available" );
    return;
  }

  n = hints;
  while ( *n != NULL )
  {
    name = snd_device_name_get_hint ( *n, "NAME" );
    if ( ( name != NULL ) && ( snd_device_name_get_hint ( *n, "IOID" ) == NULL ) )
    {
      // qDebug () << Q_FUNC_INFO << QString ( name );
      QString cardHint = QString ( name ).split ( "," ).first();
      QString infoHint = QString ( snd_device_name_get_hint ( *n, "DESC" ) );
      if ( ! buffer.contains ( cardHint.remove ( pattern ) ) )
      {
        int index = snd_card_get_index ( cardHint.toAscii().data() );
        if ( index >= 0 )
          hwIndex = QString ( "hw=%1" ).arg ( QString::number ( index ) );

        QListWidgetItem* item = new QListWidgetItem ( deviceList );
        item->setText ( QString ( "%1 \"%2\" %3" ).arg ( cardHint, infoHint.remove ( cleaner ), hwIndex ) );
        item->setData ( Qt::UserRole, cardHint );
        item->setData ( Qt::ToolTipRole, hwIndex );
        item->setData ( Qt::StatusTipRole, infoHint.remove ( cleaner ) );
        deviceList->addItem ( item );
        buffer << cardHint;
        hwIndex.clear();
      }
    }
    ++n;
  }
  snd_device_name_free_hint ( hints );

  buffer.clear();
}

const QString AlsaAudioDeviceDialog::getCard ()
{
  return deviceList->currentItem()->data ( Qt::UserRole ).toString();
}

void AlsaAudioDeviceDialog::setCard ( const QString &c )
{
  for ( int r = 0; r < deviceList->count(); r++ )
  {
    if ( deviceList->item ( r )->data ( Qt::UserRole ).toString() == c )
    {
      deviceList->setCurrentRow ( r );
      break;
    }
  }
}

const AlsaAudioDevice AlsaAudioDeviceDialog::cardInfo ()
{
  QListWidgetItem* item = deviceList->currentItem();
  AlsaAudioDevice d ( item->data ( Qt::ToolTipRole ).toString(),
                      item->data ( Qt::UserRole ).toString(),
                      item->data ( Qt::StatusTipRole ).toString() );
  return d;
}

AlsaAudioDeviceDialog::~AlsaAudioDeviceDialog()
{}

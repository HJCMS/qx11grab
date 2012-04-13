/*
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
*/

#include "abstractaudiodialog.h"

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

AbstractAudioDialog::AbstractAudioDialog ( QWidget * parent )
    : QDialog ( parent )
{
  setObjectName ( QLatin1String ( "AbstractAudioDialog" ) );
  setMinimumWidth ( 400 );
  setSizeGripEnabled ( true );

  QVBoxLayout* vLayout = new QVBoxLayout ( this );

  vLayout->addWidget ( new QLabel ( trUtf8 ( "Available Cards:" ) ) );

  m_deviceListWidget = new QListWidget ( this );
  vLayout->addWidget ( m_deviceListWidget );

  m_buttonBox = new QDialogButtonBox ( Qt::Horizontal , this );
  m_buttonBox->setStandardButtons ( ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel ) );
  m_buttonBox->setCenterButtons ( true );
  vLayout->addWidget ( m_buttonBox );

  setLayout ( vLayout );

  connect ( m_buttonBox, SIGNAL ( accepted () ), this, SLOT ( accept() ) );
  connect ( m_buttonBox, SIGNAL ( rejected () ), this, SLOT ( reject() ) );
}

/**
* Aktueller Karten Schnittstellen Name
*/
const QString AbstractAudioDialog::getCard ()
{
  return m_deviceListWidget->currentItem()->data ( Qt::UserRole ).toString();
}

/**
* Setzt die karte für die Schnittstelle
*/
void AbstractAudioDialog::setCard ( const QString &c )
{
  for ( int r = 0; r < m_deviceListWidget->count(); r++ )
  {
    if ( m_deviceListWidget->item ( r )->data ( Qt::UserRole ).toString() == c )
    {
      m_deviceListWidget->setCurrentRow ( r );
      break;
    }
  }
}

/**
* Aktueller Karten Informationen
*/
const AudioDevice AbstractAudioDialog::cardInfo ()
{
  QListWidgetItem* item = m_deviceListWidget->currentItem();
  if ( ! item )
    return AudioDevice ( "default", "default", trUtf8 ( "Fallback Interface" ) );

  return AudioDevice ( item->data ( Qt::ToolTipRole ).toString(),
                       item->data ( Qt::UserRole ).toString(),
                       item->data ( Qt::StatusTipRole ).toString() );
}

AbstractAudioDialog::~AbstractAudioDialog()
{}

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
**/

#include "opensounddialog.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

OpenSoundDialog::OpenSoundDialog ( QWidget * parent )
    : AbstractAudioDialog ( parent )
{
  setObjectName ( QLatin1String ( "OpenSoundDialog" ) );
  /*: WindowTitle */
  setWindowTitle ( trUtf8 ( "Open Sound System (OSS)" ) );
  m_deviceListWidget->clear();
  initInterface();
}

void OpenSoundDialog::initInterface()
{
  QDir dir ( "/dev" );
  QStringList filters;
  filters << "*dsp" << "dsp*" << "adsp*";
  dir.setNameFilters ( filters );
  dir.setFilter ( ( QDir::CaseSensitive | QDir::System ) );
  dir.setSorting ( QDir::Name | QDir::Size );
  foreach ( QFileInfo info, dir.entryInfoList() )
  {
    QListWidgetItem* item = new QListWidgetItem ( m_deviceListWidget );
    item->setText ( trUtf8( "%1 OSS Device: %2" ).arg ( info.baseName(), info.absoluteFilePath() ) );
    item->setData ( Qt::UserRole, info.absoluteFilePath() );
    /*: ToolTip */
    item->setData ( Qt::ToolTipRole, trUtf8 ( "Open Sound System (OSS) Device" ) );
    item->setData ( Qt::StatusTipRole, trUtf8 ( "Open Sound System (OSS) Device" ) );
    m_deviceListWidget->addItem ( item );
  }
}

OpenSoundDialog::~OpenSoundDialog()
{}

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

#include "commandpreview.h"
#include "commandlineedit.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QRegExp>

/* QtGui */
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

CommandPreview::CommandPreview ( QWidget * parent )
    : QWidget ( parent )
{
  setObjectName ( QLatin1String ( "commandpreview" ) );
  setContentsMargins ( 0, 2, 0, 2 );

  QVBoxLayout* layout = new QVBoxLayout ( this );
  layout->setContentsMargins ( 0, 0, 0, 0 );

  layout->addWidget ( new QLabel ( trUtf8 ( "Display the current FFmpeg command." ), this ) );

  m_commandLineEdit = new CommandLineEdit ( this );
  layout->addWidget ( m_commandLineEdit );

  setLayout ( layout );

  connect ( m_commandLineEdit, SIGNAL ( dataSaved ( const QStringList & ) ),
            this, SIGNAL ( dataSaved ( const QStringList & ) ) );
}

/**
*
*/
void CommandPreview::setCommandLine ( QStringList &list )
{
  QString buf ( list.join ( " " ) );
  QString cmd = buf.replace ( QRegExp ( "[\\s\\t]+\\-" ), QString::fromUtf8 ( "#-" ) );
  m_commandLineEdit->setData ( cmd.split ( "#" ) );
}

CommandPreview::~CommandPreview()
{}
